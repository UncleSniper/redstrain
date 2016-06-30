#!/usr/bin/python3

import os

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
    def getComponentProperties(self):
        return os.path.join(self.basedir, 'component.properties')
    def getDependSource(self):
        if self.ctype == CT_TOOL:
            dsbase = 'depend'
        else:
            dsbase = 'modinfo'
        return os.path.join(self.basedir, dsbase + '.cpp')
    def buildNormalClosure(self):
        pass #TODO
    def buildBlobfulClosure(self):
        pass #TODO

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
        dependables[c.name] = c
        checkables.append(c)
    elif c.ctype == CT_TOOL:
        tools.append(c)
        checkables.append(c)
    elif c.ctype == CT_DATA:
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
        printIssue("Component '" + c.basedir + "' does not have a 'component.properties'.")

for c in components:
    c.buildNormalClosure()
    c.buildBlobfulClosure()

#TODO
