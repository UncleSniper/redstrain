#!/usr/bin/python3

import os, re

havePrintedHeader = False

def printIssue(issue, isFollowup):
	global havePrintedHeader
	if not havePrintedHeader:
		print('The following issues were detected:')
		havePrintedHeader = True
	if isFollowup:
		print('  ' + issue)
	else:
		print('* ' + issue)

CT_LIBRARY = 0
CT_TOOL = 1
CT_DATA = 2

class Component(object):
	def __init__(self, basedir, ctype):
		self.basedir = basedir
		self.name = os.path.basename(basedir)
		self.ctype = ctype
		self.explicitNormal = {}
		self.explicitBlobful = {}
		self.transitiveNormal = {}
		self.transitiveBlobful = {}
		self.depmacroName = None
		self.depHumanName = None
	def getComponentProperties(self):
		return os.path.join(self.basedir, 'component.properties')
	def getDependSource(self):
		if self.ctype == CT_TOOL:
			dsbase = 'depend'
		else:
			dsbase = 'modinfo'
		return os.path.join(self.basedir, 'src', dsbase + '.cpp')
	def getDependHeader(self):
		return os.path.join(self.basedir, 'src', 'modinfo.hpp')
	def getDependencyProperties(self):
		return os.path.join(self.basedir, 'dependency.properties')
	def buildNormalClosure(self, dependables):
		newExplicit = {}
		for cname in self.explicitNormal:
			if cname in dependables:
				depend = dependables[cname]
				if depend.ctype == CT_DATA:
					printIssue("Component '" + self.basedir + "' has '" + depend.basedir
							+ "' in its 'depend.modules', but blobless dependencies cannot refer to blobs.", False)
				newExplicit[cname] = depend
				self.transitiveNormal[cname] = depend
				self._followNormalClosure(depend, dependables)
			else:
				printIssue("Component '" + self.basedir + "' depends on '" + cname
						+ "' as per its 'depend.modules', but there is no such thing.", False)
		self.explicitNormal = newExplicit
	def _followNormalClosure(self, component, dependables):
		if component is self:
			printIssue("Component '" + self.basedir + "' ultimately depends on itsself.", False)
		for cname in component.explicitNormal:
			if cname in dependables and cname not in self.transitiveNormal:
				depend = dependables[cname]
				self.transitiveNormal[cname] = depend
				self._followNormalClosure(depend, dependables)
	def buildBlobfulClosure(self, dependables):
		for cname in self.explicitNormal:
			self.explicitBlobful[cname] = True
		newExplicit = {}
		for cname in self.explicitBlobful:
			if cname in dependables:
				depend = dependables[cname]
				newExplicit[cname] = depend
				self.transitiveBlobful[cname] = depend
				self._followBlobfulClosure(depend, dependables)
			else:
				printIssue("Component '" + self.basedir + "' depends on '" + cname
						+ "' as per its 'blobful.depend.modules', but there is no such thing.", False)
		self.explicitBlobful = newExplicit
	def _followBlobfulClosure(self, component, dependables):
		if component is self:
			printIssue("Component '" + self.basedir + "' ultimately depends on itsself.", False)
		for cname in component.explicitBlobful:
			if cname in dependables and cname not in self.transitiveBlobful:
				depend = dependables[cname]
				self.transitiveBlobful[cname] = depend
				self._followBlobfulClosure(depend, dependables)
	def getL10NComponent(self):
		if self.ctype != CT_LIBRARY:
			return None
		cname = self.name + '-l10n'
		if os.path.isdir(os.path.join('data', cname)):
			return cname
		else:
			return None
	def getNamespace(self):
		cppath = self.getComponentProperties()
		if self.ctype == CT_DATA and os.path.isfile(cppath):
			f = open(cppath, 'r')
			for line in f:
				if '=' not in line:
					continue
				k, v = line.rstrip('\r\n').split('=', 1)
				if k == 'generated.namespace':
					f.close()
					return v
			f.close()
		if self.ctype == CT_DATA and self.name.endswith('-l10n'):
			return 'redengine::' + self.name[:-5] + '::l10n'
		else:
			return 'redengine::' + self.name
	def canGenerateDependSource(self):
		for cname in self.transitiveNormal:
			depend = self.transitiveNormal[cname]
			if depend.depmacroName is None or depend.depHumanName is None:
				return False
		return self.depHumanName is not None

def getAllComponents():
	components = []
	for subdir, ctype in [
		('modules', CT_LIBRARY),
		('tools', CT_TOOL),
		('data', CT_DATA)
	]:
		for cname in os.listdir(subdir):
			components.append(Component(os.path.join(subdir, cname), ctype))
	return components

def splitProperty(spec):
	result = []
	for seg in spec.split(','):
		seg = seg.strip()
		if len(seg):
			result.append(seg)
	return result

dependables = {}
checkables = []
tools = []

components = getAllComponents()

for c in components:
	if c.ctype == CT_LIBRARY:
		if c.name in dependables:
			printIssue("There are multiple components named '" + c.name + "'.", False)
		else:
			dependables[c.name] = c
		checkables.append(c)
	elif c.ctype == CT_TOOL:
		tools.append(c)
		checkables.append(c)
	elif c.ctype == CT_DATA:
		if c.name in dependables:
			printIssue("There are multiple components named '" + c.name + "'.", False)
		else:
			dependables[c.name] = c
	else:
		raise ValueError('Unrecognized component type: ' + repr(c.ctype))
	cpropp = c.getComponentProperties()
	if os.path.isfile(cpropp):
		cpropf = open(cpropp, 'r')
		for cpropl in cpropf:
			if '=' not in cpropl:
				continue
			cpropkey, cpropvalue = cpropl.rstrip('\r\n').split('=', 1)
			if cpropkey == 'depend.modules':
				for cname in splitProperty(cpropvalue):
					c.explicitNormal[cname] = True
			elif cpropkey == 'blobful.depend.modules':
				for cname in splitProperty(cpropvalue):
					c.explicitBlobful[cname] = True
		cpropf.close()
	else:
		printIssue("Component '" + c.basedir + "' does not have a 'component.properties'.", False)

for c in components:
	c.buildNormalClosure(dependables)
	c.buildBlobfulClosure(dependables)

for c in components:
	if c.ctype == CT_DATA:
		for cname in c.transitiveBlobful:
			if cname in dependables:
				depend = dependables[cname]
				if depend.ctype == CT_DATA:
					printIssue("Component '" + c.basedir + "' depends on '" + depend.basedir
							+ "', but blobs should not depend on each other.", False)
	elif c.ctype == CT_TOOL:
		for cname in c.transitiveNormal:
			if cname in dependables:
				depend = dependables[cname]
				if depend.ctype == CT_LIBRARY:
					ccname = depend.getL10NComponent()
					if ccname is not None and ccname in dependables and ccname not in c.transitiveBlobful:
						printIssue("Component '" + c.basedir + "' depends on '" + depend.basedir
								+ "', but not on '" + dependables[ccname].basedir + "'.", False)
		if 'text' in c.transitiveNormal and 'charsets' not in c.transitiveBlobful:
			printIssue("Component '" + c.basedir + "' depends on mod_text, but not on blob_charsets.", False)

def makeNamespacePair(ns):
	begin = ''
	end = ''
	for seg in ns.split('::'):
		seg = seg.strip()
		if len(seg):
			begin += 'namespace ' + seg + ' {\n'
			end += '}'
	if len(begin):
		begin += '\n'
	if len(end):
		end = '\n' + end + '\n'
	return begin, end

def genDependSource(component):
	dspath = component.getDependSource()
	f = open(dspath, 'w')
	for cname in component.transitiveNormal:
		f.write('#include <redstrain/' + cname + '/modinfo.hpp>\n')
	f.write('\n')
	if component.ctype == CT_LIBRARY:
		f.write('#include "modinfo.hpp"\n\n')
		begin, end = makeNamespacePair(component.getNamespace())
		f.write(begin)
		f.write('\tREDSTRAIN_DEFINE_MODULE_VERSION(' + component.depmacroName + ')\n\n')
		indent = '\t'
	else:
		end = ''
		indent = ''
	for cname in component.transitiveNormal:
		depend = component.transitiveNormal[cname]
		shortdhn = depend.depHumanName
		if '/' in shortdhn:
			shortdhn = shortdhn[shortdhn.find('/') + 1:]
		f.write(indent + 'REDSTRAIN_DEPEND_MODULE(' + shortdhn + '\n')
		f.write(indent + '\t\t' + component.depHumanName + ', ' + depend.depHumanName + ', '
				+ depend.depmacroName + ', ::' + depend.getNamespace() + ')\n')
	f.write(end)
	f.close()

def genDependHeader(component):
	dhpath = component.getDependHeader()
	begin, end = makeNamespacePair(component.getNamespace())
	f = open(dhpath, 'w')
	f.write('#ifndef ' + component.depmacroName + '_MODINFO_HPP\n')
	f.write('#define ' + component.depmacroName + '_MODINFO_HPP\n\n')
	f.write('#include <redstrain/redmond/LibraryDependency.hpp>\n\n')
	f.write('#include "api.hpp"\n\n')
	f.write('#define ' + component.depmacroName + '_STATIC_VERSION_MAJOR 0u\n')
	f.write('#define ' + component.depmacroName + '_STATIC_VERSION_MINOR 1u\n\n')
	f.write(begin)
	f.write('\tREDSTRAIN_DECLARE_MODULE_VERSION(' + component.depmacroName + ')\n')
	f.write(end + '\n')
	f.write('#endif /* ' + component.depmacroName + '_MODINFO_HPP */\n')
	f.close()

RE_DEPMACRO_NAME = re.compile('^[ \t]*REDSTRAIN_DEFINE_MODULE_VERSION\\(([A-Z0-9_]+)\\)$')
RE_DEPDECL_UPPER = re.compile('^[ \t]*REDSTRAIN_DEPEND_MODULE\\([^,]+,$')
RE_DEPDECL_LOWER = re.compile('^[ \t]*[^,]+, [^,]+, ([A-Z0-9_]+), ::[^,]+\\)$')

for c in components:
	dspath = c.getDependSource()
	if os.path.isfile(dspath):
		f = open(dspath, 'r')
		for line in f:
			line = line.rstrip('\r\n')
			m = RE_DEPMACRO_NAME.search(line)
			if m is not None:
				c.depmacroName = m.group(1)
				break
		f.close()
		if c.ctype != CT_TOOL and c.depmacroName is None:
			printIssue("Component '" + c.basedir + "' fails to use REDSTRAIN_DEFINE_MODULE_VERSION in its '"
					+ dspath + "'.", False)
			if c.ctype == CT_LIBRARY:
				c.depmacroName = 'REDSTRAIN_MOD_' + c.name.upper()
	dppath = c.getDependencyProperties()
	if os.path.isfile(dppath):
		f = open(dppath, 'r')
		for line in f:
			if '=' not in line:
				continue
			k, v = line.rstrip('\r\n').split('=', 1)
			if k == 'human.readable.name':
				c.depHumanName = v
		f.close()

for c in components:
	dspath = c.getDependSource()
	if not os.path.isfile(dspath):
		printIssue("Component '" + c.basedir + "' is missing a '" + dspath + "'.", False)
		if c.canGenerateDependSource():
			printIssue('-> I will generate one for you.', True)
			genDependSource(c)
			dhpath = c.getDependHeader()
			if c.ctype == CT_LIBRARY and not os.path.isfile(dhpath):
				printIssue("-> No '" + dhpath +  "' exists, either -- I well generate thatm too.", True)
				genDependHeader(c)
		else:
			printIssue('-> Cannot generate one, as not all necessary information is known.', True)

for c in components:
	dspath = c.getDependSource()
	if os.path.isfile(dspath):
		declaredDeps = {}
		mayBeLower = False
		f = open(dspath, 'r')
		for line in f:
			line = line.rstrip('\r\n')
			if not mayBeLower:
				m = RE_DEPDECL_UPPER.match(line)
				if m is not None:
					mayBeLower = True
			else:
				m = RE_DEPDECL_LOWER.match(line)
				if m is not None:
					declaredDeps[m.group(1)] = True
					mayBeLower = False
				elif RE_DEPDECL_UPPER.match(line) is None:
					mayBeLower = False
		f.close()
		for cname in c.transitiveNormal:
			depend = c.transitiveNormal[cname]
			if depend.depmacroName is not None and depend.depmacroName not in declaredDeps:
				printIssue("Component '" + c.basedir + "' does not declare its dependency on '"
						+ depend.basedir + "' in its '" + dspath + "'.", False)
