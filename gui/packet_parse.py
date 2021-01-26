from pycparser import c_parser, c_ast

from subprocess import check_output
import struct


header_file_text = check_output(["gcc", "-E", "../rover/packets.h"])
parser = c_parser.CParser()
ast = parser.parse(header_file_text.decode("utf-8"), filename='<none>')

def struct_ast_to_list(d):
    ls = []
    for decl in d.decls:
        if type(decl.type) != c_ast.TypeDecl:
            continue
        ls.append((decl.type.declname, tuple(decl.type.type.names)))
    return tuple(ls)

def extract_structs(ast):
    structs = []
    if type(ast) == c_ast.Typedef:
        structs.append((ast.name, struct_ast_to_list(ast.type.type)))
    for _, c in ast.children():
        structs += extract_structs(c)
    return tuple(structs)

structs = extract_structs(ast)

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

def pack(type_name, data):
    def assoc_i(ls, key):
        for i, (a, b) in enumerate(ls):
            if a == key:
                return b, i
    types, type_number = assoc_i(structs, type_name)
    if types == None:
        raise Exception(type_name + ' is not a type')

    assert len(types) == 1 + len(kwargs), 'you have the wrong number of entries for this struct'
    for (correct_name, types_list) in types[1:]
        if correct_name not in data:
            raise Exception('data dict missing key ', correct_name)
    foo = type_list_to_struct_spec(types)
    args = [type_number,] + [data[key] for (key, _) in types[1:]]
    return struct.pack(foo, *args)

def packet_to_stream(packet):
    '''Converts bytestring into bytestring for uart streaming. That is,
    prepends length byte and appends checksum.
    '''
    def n2b(n):
        'convert int (less that 256) to bytestring with the value'
        return (n).to_bytes(1, 'big')
    size = len(packet)
    checksum = sum(packet) % 256
    return n2b(size) + packet + n2b(checksum)

def stream_to_packet(stream):
    length = stream[0]
    check_byte = stream[-1]
    packet = stream[1:-1]
    if len(payload) != length:
        raise Exception("Incorrect length byte!")
    if sum(list(packet)) != check_byte:
        raise Exception("Check checksum failed")
    return packet
    
