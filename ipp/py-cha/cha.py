#!/usr/bin/env python3.6
import re
import sys
import getopt
import os

class IPPDir:
    def __init__(self, name):
        self.files = list()
        self.name = name 
    def do_it(self):
        for f in self.files:
           f.read_from_file() 
    def __str__(self):
        string = "<functions dir=\"" + self.name + "\">\n"
        for f in self.files:
            string += f.__str__() 
        string += "</functions>"
        return string
    def __repr__(self):
        return self.__str__()

class IPPFile:
    def __init__(self, name):
        self.functions = list()
        self.funcDict = dict()
        self.name = name 

    def read_from_file(self):
        tokens = list()
        try:
            with open(self.name, 'r') as f:
                code = remove_comments(f.read())
                for token in tokenize(code):
                    if token[0] != 'whitespace':
                        tokens.append(id_to_reserved_word(token))
        except Exception as err:
            panic(err, 2)
        
        self.read_from_tokens(tokens)

    def process(self, part_a, part_b):
        if globfig['inline'] and part_a[0][0] == 'inline':
            return
        # Get function name
        if part_a[len(part_a) - 1][0] != 'identifier':
            return None
        name = part_a[len(part_a) - 1][1]

        # Get return value
        ret_val = list()

        for x in range(0, len(part_a) - 1):
            if part_a[x][0] == 'identifier':
                ret_val.append(part_a[x][1])
            else:
                ret_val.append(token_to_string(part_a[x][0]))

        f = IPPFunc(name, os.path.basename(self.name), " ".join(ret_val))

        pnumber = 1
        p = list()

        for x in range(0, len(part_b) - 1):
            if part_b[x][0] == 'comma':
                f.params.append(IPPParam(pnumber, " ".join(p)))
                pnumber += 1
                p = list()
            else:
                if part_b[x][0] == 'identifier':
                    p.append(part_b[x][1])
                else:
                    p.append(token_to_string(part_b[x][0]))

        if len(p) != 0:
            f.params.append(IPPParam(pnumber, " ".join(p)))
            
        if globfig["nodup"] and (name in self.funcDict):
            return
        
        self.functions.append(f)
        self.funcDict[name] = True
        
    def read_from_tokens(self, tokens):
        start = 0
        position = 0
        bracket_counter = 0;
        for x in range(0, len(tokens)):
            if tokens[x][0] == 'open_bracket':
                if bracket_counter == 0:
                    position = x
                bracket_counter += 1
            if tokens[x][0] == 'close_bracket':
                bracket_counter -= 1
            if tokens[x][0] == 'semicolon':
                self.process(tokens[start:position], tokens[position + 1:x - 1])
                start = x + 1

    def __str__(self):
        out = ""
        for f in self.functions:
            out += f.__str__()
        return out
    def __repr__(self):
        return self.__str__()

class IPPFunc:
    def __init__(self, name, filename, rettype):
        self.params = list()
        self.name = name
        self.filename = filename
        self.rettype = rettype
        self.varargs = 'false'
    def __str__(self):
        if len(self.params) > globfig["maxpar"]:
            return ""
        string = globfig["indent"] + "<function file=\"" + self.filename + "\" name=\"" + self.name + "\" varargs=\"" + self.varargs + "\" rettype=\"" + self.rettype + "\">\n"
        for f in self.params:
            string += globfig["indent"] * 2 + f.__str__() + "\n"
        string += globfig["indent"] + "</function>\n"
        return string
    def __repr__(self):
        return self.__str__()
        
class IPPParam:
    def __init__(self, number, paramtype):
        self.number = number
        self.paramtype = paramtype
    def __str__(self):
        return "<param number=\"" + str(self.number) + "\" type=\"" + self.paramtype + "\" />"
    def __repr__(self):
        return self.__str__()

directory = IPPDir("Placeholder")
directory.files.append(IPPFile("FileName"))

token_pattern = r"""
(?P<identifier>[a-zA-Z_][a-zA-Z0-9_]*)
|(?P<comma>,)
|(?P<open_bracket>[\(])
|(?P<close_bracket>[\)])
|(?P<whitespace>[\s\n]+)
|(?P<slash>[/])
|(?P<pointer>[\*])
|(?P<semicolon>[;])
"""

token_re = re.compile(token_pattern, re.VERBOSE)
reserved_words = dict()
reserved_words["char"] = True
reserved_words["const"] = True
reserved_words["double"] = True
reserved_words["float"] = True
reserved_words["inline"] = True
reserved_words["int"] = True
reserved_words["long"] = True
reserved_words["short"] = True
reserved_words["signed"] = True
reserved_words["static"] = True
reserved_words["struct"] = True
reserved_words["unsigned"] = True
reserved_words["void"] = True

tknz = dict()
tknz["open_bracket"] = "("
tknz["close_bracket"] = ")"
tknz["slash"] = "/"
tknz["pointer"] = "*"
tknz["semicolon"] = ";"

def token_to_string(string):
    if string in reserved_words:
        return string
    else:
        return tknz[string]

def remove_comments(string):
    pattern = r"(\".*?\"|\'.*?\')|(/\*.*?\*/|//[^\r\n]*$|#[^\r\n]*$|^[\w\s]+{.*?};)"
    # first group captures quoted strings (double or single)
    # second group captures comments (//single-line or /* multi-line */)
    regex = re.compile(pattern, re.MULTILINE|re.DOTALL)
    def _replacer(match):
        # if the 2nd group (capturing comments) is not None,
        # it means we have captured a non-quoted (real) comment string.
        if match.group(2) is not None:
            return "" # so we will return empty to remove the comment
        else: # otherwise, we will return the 1st group
            return match.group(1) # captured quoted-string
    return regex.sub(_replacer, string)

class TokenizerException(Exception): pass

def tokenize(text):
    pos = 0
    while True:
        m = token_re.match(text, pos)
        if not m: break
        pos = m.end()
        tokname = m.lastgroup
        tokvalue = m.group(tokname)
        yield tokname, tokvalue
    if pos != len(text):
        raise TokenizerException('tokenizer stopped at pos %r of %r' % (
            pos, len(text)))

def id_to_reserved_word(token):
    if token[1] in reserved_words:
        return (token[1],"")
    return token

# Learning to walk again, I believe I've waited long enough, where do I begin?
def walk(root):
    dirzYolo = list()

    for root, dirs, files in os.walk(root):
        direktoria = IPPDir(root + '/')

        for f in files:
            filename, extension = os.path.splitext(f)
            if extension == '.h':
                direktoria.files.append(IPPFile(root + '/' + f))

        dirzYolo.append(direktoria)

    return dirzYolo

def panic(message, code):
    print(message, file=sys.stderr)
    sys.exit(code)

def print_help():
    print("CHA in Python. Usage:")
    print("\t--help - prints help")
    print("\t--input=[file] - defines input file")
    print("\t--output=[file] - defines output file")
    print("\t--pretty-xml=[indent] - sets indentation of each xml subelement in number of spaces")
    print("\t--no-inline - ignore inline functions")
    print("\t--max-par=[n] - print only functions with less than n params")
    print("\t--no-duplicates - print only last function if there are any with same name")
    print("\t--remove-whitespace - rettype and type are printed without withspaces")

try:
    optlist, args = getopt.getopt(sys.argv[1:], '', ['help', 'output=', 'input=','pretty-xml=','no-inline','no-duplicates','max-par=','remove-whitespaces'])
except getopt.GetoptError as err:
    panic(err, 2)

optlist = dict(optlist)

if '--help' in optlist:
    print_help()

globfig = dict()
globfig["indent"] = "    "
globfig["inline"] = False
globfig["maxpar"] = 100000000
globfig["nodup"] = False

if '--pretty-xml' in optlist:
    if optlist['--pretty-xml'].isnumeric():
        globfig['indent'] = " " * int(optlist['--pretty-xml'])
    else:
        panic("Number expected in option --pretty-xml", 1)

if '--no-inline' in optlist:
    globfig["inline"] = True
    
if '--no-duplicates' in optlist:
    globfig["nodup"] = True
    
if '--no-duplicates' in optlist:
    globfig["nodup"] = True

if '--max-par' in optlist:
    if optlist['--max-par'].isnumeric():
        globfig['maxpar'] = int(optlist['--max-par'])
    else:
        panic("Number expected in option --max-par", 1)

if '--input' in optlist:
    if os.path.isfile(optlist['--input']):
        defaultDir = IPPDir("")
        defaultDir.files.append(IPPFile(optlist['--input']))
        dirzYolo = list()
        dirzYolo.append(defaultDir)
        d = ""
    elif os.path.isdir(optlist['--input']):
        d = optlist['--input']
else:
    d = "."

if d != "":
    dirzYolo = walk(d)

for d in dirzYolo:
    d.do_it()
    if '--output' in optlist:
        try:
            with open(optlist['--output'], 'w') as f:
                f.write(d.__str__())
        except Exception as err:
            panic(err, 3)
    else:
        print(d)

# suborikcmuqycmuk = IPPFile("nieco.h")
# suborikcmuqycmuk.read_from_tokens(tokens)
# 
# print(suborikcmuqycmuk.__str__())
# 