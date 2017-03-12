#!/usr/bin/env python3.6
import re

class IPPDir:
    def __init__(self, name):
        self.files = list()
        self.name = name 

class IPPFile:
    def __init__(self, name):
        self.functions = list()
        self.name = name 
    def process(self, part_a, part_b):
        print(part_a)
        print(part_b)
        if tokens[len(part_a) - 1][0] != 'identifier':
            print("FUCK UP ! Bitch!")
            return None
        name = tokens[len(part_a) - 1][1]
        ret_val = ""

        for x in range(0, len(part_a) - 1):
            if part_a[x][0] == 'identifier':
                ret_val = ret_val + ' ' + part_a[x][1]
            else:
                ret_val = ret_val + ' ' + part_a[x][1]

        self.functions.append(IPPFunc(name, ret_val))
        
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


               # f = self.get_name_return(tokens[start_position:position])
               # if f == None:
               #     print(tokens[start_position:position])
               # else:
               #     self.functions.append(IPPFunc(f[0],f[1]))
class IPPFunc:
    def __init__(self, name, rettype):
        self.params = list()
        self.name = name
        self.rettype = rettype
        self.varargs = 'false'
    def __str__(self):
        string = "<function file=\"" + self.name + "\" name=\"" + self.name + "\" varargs=\"" + self.varargs + "\" rettype=\"" + self.rettype + "\">\n"
        string += "</function>\n"
        return string
    def __repr__(self):
        return self.__str__()
        
class IPPParam:
    def __init__(self, number, paramtype):
        self.number = number
        self.paramtype = paramtype
    def __str__(self):
        return "<param number=\"" + self.number + "\" type=\"" + self.paramtype + "\" />"

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
reserved_words["int"] = True
reserved_words["long"] = True
reserved_words["short"] = True
reserved_words["signed"] = True
reserved_words["static"] = True
reserved_words["struct"] = True
reserved_words["unsigned"] = True
reserved_words["void"] = True

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

tokens = list()
with open('nieco.h', 'r') as f:
    code = remove_comments(f.read())
    print(code)
    for token in tokenize(code):
        if token[0] != 'whitespace':
            tokens.append(id_to_reserved_word(token))

suborikcmuqycmuk = IPPFile("nieco.h")
suborikcmuqycmuk.read_from_tokens(tokens)

print(suborikcmuqycmuk.functions)

