#!/usr/bin/env python3.6

##              ##
### ###    ### ###
#### xkulic03 ####
### ###    ### ###
##              ##

#          ,_---~~~~~----._         
#   _,,_,*^____      _____``*g*\"*, 
#  / __/ /'     ^.  /      \ ^@q   f 
# [  @f | @))    |  | @))   l  0 _/  
#  \`/   \~____ / __ \_____/    \   
#   |           _l__l_           I   
#   }          [______]           I  
#   ]            | | |            |  
#   ]             ~ ~             |  
#   |                            |   
#    |                           |   

#    Golang for 1st project please.
#       Make FIT great again!

import re
import sys
import getopt
import os

# TODO: 
# fix removing whitespaces
# pointers to functions as parameters 

class IPPDir:
    def __init__(self, name):
        self.files = list()
        self.name = name 
    def do_it   (self):
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
                code = remove_bodies(code)
                try:
                    for token in tokenize(code):
                        tokens.append(id_to_reserved_word(token))
                except TokenizerException as err:
                    panic(err, 4)
                
                        
        except Exception as err:
            panic(err, 2)
        self.read_from_tokens(tokens)

    def process(self, part_a, part_b):
        if is_inline(part_a):
            return

        funNamePos = 0
        # Get function name
        for x in range(len(part_a) - 1, 0, -1):
            if part_a[x][0] == 'identifier':
                name = part_a[x][1]
                funNamePos = x
                break

        # Get return value
        ret_val = list()

        bracket_counter = 0
        
        for x in range(0, funNamePos):
            if part_a[x][0] == 'identifier' or part_a[x][0] == 'whitespace':
                ret_val.append(part_a[x][1])
            else:
                ret_val.append(token_to_string(part_a[x][0]))

        varargs = has_vararg(part_b)
        f = IPPFunc(name, self.name, get_normalized_ret(ret_val), varargs)

        pnumber = 1
        p = list()

        bracket_counter = 0

        for item in part_b:
            if bracket_counter == 0 and item[0] == 'comma':
                f.params.append(IPPParam(pnumber, get_normalized_params(p)))
                pnumber += 1
                p = list()
            else:
                if item[0] == 'open_bracket':
                    bracket_counter += 1
                elif item[0] == 'close_bracket': 
                    bracket_counter -= 1

                p.append(item)

        if len(p) != 0 and varargs == 'no':
            norm_params = get_normalized_params(p)
            if not norm_params == 'void':
                f.params.append(IPPParam(pnumber, get_normalized_params(p)))
                pnumber += 1
            
        if globfig["nodup"] and (name in self.funcDict):
            return
        
        self.functions.append(f)
        self.funcDict[name] = True
        
    def read_from_tokens(self, tokens):
        start = 0
        position = 0
        bracket_counter = 0
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
    def __init__(self, name, filename, rettype, varargs):
        self.params = list()
        self.name = name
        self.filename = filename
        self.rettype = rettype
        self.varargs = varargs
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

token_pattern = r"""
(?P<identifier>[a-zA-Z_][a-zA-Z0-9_]*)
|(?P<comma>,)
|(?P<vararg>\.\.\.)
|(?P<open_bracket>[\(])
|(?P<close_bracket>[\)])
|(?P<open_array>[\[])
|(?P<close_array>[\]])
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
reserved_words["extern"] = True
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
tknz["open_array"] = "["
tknz["close_array"] = "]"
tknz["slash"] = "/"
tknz["pointer"] = "*"
tknz["semicolon"] = ";"
tknz["vararg"] = "vararg"
tknz["comma"] = ","

def has_vararg(params):
    for i in params:
        if i[0] == 'vararg':
            return "yes"
    return "no"

def is_inline(params):
    if globfig['inline'] == False:
        return False
    
    for x in range(0,len(params)):
        if params[x][0] == 'inline':
            return True

    return False

def token_to_string(string):
    if string in reserved_words:
        return string
    else:
        return tknz[string]

def _replacer_comments(match):
        if match.group(2) is not None:
            return "" 
        else: 
            return match.group(1)

def _replacer_bodies(match):
        if match.group(1) is not None:
            return ";" 
        else: 
            return match.group(1) 

def remove_comments(string):
    pattern = r"(\".*?\"|\'.*?\')|(/\*.*?\*/|//[^\r\n]*$|#[^\r\n]*$|^[\w\s]+{.*?};)"
    regex = re.compile(pattern, re.MULTILINE|re.DOTALL)
    
    return regex.sub(_replacer_comments, string)

def remove_bodies(string):
    pattern = r"({.*?})"
    regex = re.compile(pattern, re.MULTILINE|re.DOTALL)
    
    return regex.sub(_replacer_bodies, string)

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

def get_normalized(val):
    asd = "".join(val)
    asd = re.sub('\s', ' ', asd)

    if globfig["rmwhite"] == True:
        asd = re.sub('\s+', ' ', asd)
        asd = re.sub('\s\*', '*', asd)
    return asd.strip()

def get_normalized_ret(val):
    nret = list()
    for item in val:
        if item != 'inline' and item != 'static':
            nret.append(item)
    return get_normalized(nret)

def get_normalized_params(val):
    for x in range(len(val) - 1, 1, -1):
        if val[x][0] == 'identifier' and val[x - 1][0] != 'struct':
            del val[x]
            break
    p = list()
    for item in val:
        if item[0] == 'identifier' or item[0] == 'whitespace':
            p.append(item[1])
        else:
            p.append(token_to_string(item[0]))
    return get_normalized(p)

# Learning to walk again, I believe I've waited long enough, where do I begin?
def walk(root):
    dirzYolo = IPPDir(root)

    for root, dirs, files in os.walk(root):
        # direktoria = IPPDir(root + '/')

        for f in files:
            filename, extension = os.path.splitext(f)
            if extension == '.h':
                ippfilename = root
                if ippfilename[len(ippfilename) - 1] != '/':
                    ippfilename += '/'
                ippfilename += f
                dirzYolo.files.append(IPPFile(ippfilename))

        # dirzYolo.append(direktoria)

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
globfig["indent"] = ""
globfig["inline"] = False
globfig["maxpar"] = 100000000
globfig["nodup"] = False
globfig["rmwhite"] = False

if '--pretty-xml' in optlist:
    if optlist['--pretty-xml'].isnumeric():
        globfig['indent'] = " " * int(optlist['--pretty-xml'])
    elif optlist['--pretty-xml'] == '':
        globfig['indent'] = "    "
    else:
        panic("Number expected in option --pretty-xml", 1)

if '--no-inline' in optlist:
    globfig["inline"] = True
    
if '--no-duplicates' in optlist:
    globfig["nodup"] = True
    
if '--no-duplicates' in optlist:
    globfig["nodup"] = True

if '--remove-whitespaces' in optlist:
    globfig["rmwhite"] = True

if '--max-par' in optlist:
    if optlist['--max-par'].isnumeric():
        globfig['maxpar'] = int(optlist['--max-par'])
    else:
        panic("Number expected in option --max-par", 1)

d = ""
dirzYolo = IPPDir

if '--input' in optlist:
    if os.path.isfile(optlist['--input']):
        dirzYolo = IPPDir("")
        dirzYolo.files.append(IPPFile(optlist['--input']))
    elif os.path.isdir(optlist['--input']):
        d = optlist['--input']
    else:
        panic("File not exists.", 2)
else:
    d = "."

if d != "":
    dirzYolo = walk(d)

dirzYolo.do_it()
if '--output' in optlist:
    try:
        with open(optlist['--output'], 'w') as f:
            f.write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>")
            f.write(dirzYolo.__str__())
    except Exception as err:
        panic(err, 3)
else:
    print("<?xml version=\"1.0\" encoding=\"UTF-8\"?>")
    print(dirzYolo)
