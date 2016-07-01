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
	def getComponentProperties(self):
		return os.path.join(self.basedir, 'component.properties')
	def getDependSource(self):
		if self.ctype == CT_TOOL:
			dsbase = 'depend'
		else:
			dsbase = 'modinfo'
		return os.path.join(self.basedir, 'src', dsbase + '.cpp')
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
			printIssue("Component '" + c.basedir + "' fails to use REDSTRAIN_DEFINE_MODULE_VERSION.", False)
	else:
		printIssue("Component '" + c.basedir + "' is missing a '" + dspath + "'.", False)

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
