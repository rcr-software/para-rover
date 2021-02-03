from pycparser import c_parser, c_ast

from subprocess import check_output
import struct

'''
Okay if you don't know anything about ASTs or such this is going to be quite a trip.
Pyparse takes in packets.h and converts it into a tree, with all the same information
but in a more computer readable way.

We use that tree to construct a mapping from the name of a struct (for instance, 'triplet_t')
to it's "type spec", the thing that python's struct library uses to know the type of each element,
such as 'iif' for two ints and a float or 'i70p' for an int and a 70 character byte array.
'''

def get_ast(filename):
    header_file_text = check_output(["gcc", "-E", filename])
    parser = c_parser.CParser()
    ast = parser.parse(header_file_text.decode("utf-8"), filename='<none>')
    return ast

type_tuple_to_struct_spec = {
    'char': 'c',
    'signed char': 'b',
    'unsigned char': 'B',
    'short': 'h',
    'unsigned short': 'H',
    'int': 'i',
    'unsigned int': 'I',
    'long': 'l',
    'unsigned long': 'L',
    'long long': 'q',
    'unsigned long long': 'Q',
    'float': 'f',
    'double': 'd'}

def typedef_ast_to_list(d):
    '''takes in a Typedef and returns a list of (field_name, struct_spec) pairs, such as
        [('type_spec', 'i'), ('a', 'i'), ('b', 'i'), ('c', 'i')]
        for triple_t'''
    out = []
    for decl in d.decls:
        if type(decl.type) == c_ast.ArrayDecl:
            # char array
            field_name = decl.type.type.declname
            assert decl.type.type.type.names == ['char'], 'only character arrays are currently supported'
            length = decl.type.dim.value
            out.append((field_name, str(length) + 'p'))

        elif type(decl.type) == c_ast.TypeDecl:
            # any other type
            field_name = decl.type.declname
            type_string = ' '.join(decl.type.type.names)
            assert type_string in type_tuple_to_struct_spec, "The following type list doesn't have a spec: " + type_string
            struct_spec = type_tuple_to_struct_spec[type_string]
            out.append((field_name, struct_spec))
        else:
            raise Exception("unknown type: " + decl.show())
    return out

def recursively_extract_all_structs(ast):
    '''Instead of finding all of the Typedef's the right way,
        we ititerate through everything until we find them. The only downside is that
        if there are other typedefs in packets.h, they'll throw a wrench in this.'''
    structs = []
    if type(ast) == c_ast.Typedef:
        # here, ast.name is, for example, 'triplet_t', and the second of the pair
        # is a list of pairs associating the name of each field with its letter code struct spec as
        # per https://docs.python.org/3/library/struct.html
        structs.append((ast.name, typedef_ast_to_list(ast.type.type)))
    for _, c in ast.children():
        structs += recursively_extract_all_structs(c)
    return structs


# Globals
ast = get_ast("../rover/packets.h")
structs = recursively_extract_all_structs(ast)

def type_list_to_struct_spec(ls):
    ''''takes [('foo', 'i'), ('bar', 'f'), ('rosco', '40p')] and returns '<if40p' '''
    return '<' + ''.join([b for _, b in ls])

def ensure_all_structs_in_size_limit(limit):
    '''Tests all struct specs to be within the 255 size limit of the packet radio'''
    for name, fields in structs:
        struct_spec = type_list_to_struct_spec(fields)
        assert struct.calcsize(struct_spec) <= limit

ensure_all_structs_in_size_limit(255)

def unpack(byte_string):
    '''Uses first 4 bytes as an integer to indicate type, and gets the nth type spec to unpack the byte string.
    Return the struct's name and a dict of field:value pairs.'''
    type_spec = struct.unpack('i', byte_string[:4])[0]
    name, types = structs[type_spec]
    struct_spec = type_list_to_struct_spec(types)
    values = struct.unpack(struct_spec, byte_string)
    return name, {key: value for (key, _), value in zip(types[1:], values[1:])}

def pack(type_name, data):
    '''Type name is, for example, 'triplet_t'. data is a dict relating that struct's fields (as
    strings) to the values you want. Note the type_spec is filled automatically.'''
    def assoc_i(ls, key):
        '''takes in list of pairs, returns second item and index when key matches first item'''
        for i, (a, b) in enumerate(ls):
            if a == key:
                return b, i
    types, type_number = assoc_i(structs, type_name)
    if types == None:
        raise Exception(type_name + ' is not a type')

    assert len(types) == 1 + len(data), 'you have the wrong number of entries for this struct'
    for (correct_field_name, types_list) in types[1:]:
        if correct_field_name not in data:
            raise Exception('data dict missing key ', correct_field_name)
    foo = type_list_to_struct_spec(types)
    args = [type_number,] + [data[key] for (key, _) in types[1:]]
    return struct.pack(foo, *args)

def test_packing():
    print(pack('string_t', {'string': b'Hello World'}))
    print(list(pack('triplet_t', {'a': 1, 'b':2, 'c':3})))
    print(list(pack('joystick_t', {'x1': 1.0, 'y1':2.0, 'x2':0.125, 'y2': -1.0})))

    # round tripping
    print(unpack(pack('string_t', {'string': b'Hello World'})))
    print(unpack(pack('triplet_t', {'a': 1, 'b':2, 'c':3})))
    print(unpack(pack('joystick_t', {'x1': 1.0, 'y1':2.0, 'x2':0.125, 'y2': -1.0})))

#test_packing()

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
    if len(packet) != length:
        raise Exception("Incorrect length byte!")
    if sum(list(packet)) != check_byte:
        raise Exception("Check checksum failed")
    return packet
