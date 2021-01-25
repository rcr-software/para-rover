from pycparser import c_parser, c_ast

from subprocess import check_output
import struct

text = check_output(["gcc", "-E", "../rover/packets.h"])
parser = c_parser.CParser()
ast = parser.parse(text.decode("utf-8"), filename='<none>')

def get_things(d):
    ls = []
    for decl in d.decls:
        if type(decl.type) != c_ast.TypeDecl:
            continue
        ls.append((decl.type.declname, tuple(decl.type.type.names)))
    return tuple(ls)

def explore(ast):
    structs = []
    if type(ast) == c_ast.Typedef:
        structs.append((ast.name, get_things(ast.type.type)))
    for _, c in ast.children():
        structs += explore(c)
    return tuple(structs)

structs = explore(ast)
#print(structs)

def assoc_i(ls, key):
    for i, (a, b) in enumerate(ls):
        if a == key:
            return b, i

def type_list_to_struct_spec(ls):
    ''''takes [('foo', ['int']), ('bar', ['float'])] and returns 'if' '''
    s = ''
    for _, type_list in ls:
        s += type_list[0][0]
    return '<' + s

def unpack(byte_string):
    type_spec = byte_string[0]
    name, types = structs[type_spec]
    types_first_one_first_letter = type_list_to_struct_spec(types)
    values = struct.unpack(types_first_one_first_letter, byte_string)
    return name, {key: value for (key, _), value in zip(types[1:], values[1:])}

def pack(type_name, **kwargs):
    types, type_number = assoc_i(structs, type_name)
    if types == None:
        raise Exception(type_name + ' is not a type')

    assert(len(types) == 1 + len(kwargs))
    for ((correct_name, types_list), (name, value)) in zip(types[1:], kwargs.items()):
        if correct_name != name:
            raise Exception('wrong kwargs, got ' + correct_name + ' ' + name + ', should be ' + str(types))
    foo = type_list_to_struct_spec(types)
    args = [type_number,] + list(kwargs.values())
    return struct.pack(foo, *args)

