import sys
import random

def bytes_to_int (b) : # Turns the bytes to an int with the bytes in the correct order
    return bytes_to_int_reverse_order(b[::-1])

def bytes_to_int_reverse_order (b) : # Turns the bytes to an int, but with the bytes reversed
    return sum([num*256**scale for num, scale in zip(b, range(len(b)))])

WIDTH = 4 # Replaced with 2**(the first byte of the file)
          # The first WIDTH bytes are removed, encluding the one used.
          # Row 0 is for formatting stuff.

depth_stack = []
regesters = {'A':0,'B':0,'C':0,'E':0}
character_input_que = ''

file_path = sys.argv[1]

src_code = b''

with open(file_path, 'rb') as file :
    src_code = file.read()
    WIDTH = 2 ** src_code[0] # Set the WIDTH to 2**(first byte) [0x02 -> 4 and 0x08 -> 256]
    # src_code = src_code[WIDTH:] # Throw out the top row -- changed my mind, let's leave it

current_line_position = WIDTH
depth_stack.append(current_line_position)

while len(depth_stack) > 0 :
    current_line_position = depth_stack.pop()
    while True :
        current_line = src_code[current_line_position:current_line_position+WIDTH-current_line_position%WIDTH]
        match current_line[0]:
            case 65: #A # Set regester A to value
                regesters['A'] = current_line[1]
            case 66: #B # Set regester B to value
                regesters['B'] = current_line[1]
            case 97: #a # Set regester A to value of a byte at the provided location
                regesters['A'] = src_code[bytes_to_int_reverse_order(current_line[1:])]
            case 98: #b # Set regester B to value of a byte at the provided location
                regesters['B'] = src_code[bytes_to_int_reverse_order(current_line[1:])]
            case 99: #c # Set the value of the byte location provided to the C regester's value
                location = bytes_to_int_reverse_order(current_line[1:])
                src_code = src_code[:location]+int.to_bytes(regesters['C'], length = 1, byteorder = 'little')+src_code[location+1:]
            case 101: #e # Set the value of the byte location provided to the E regester's value
                location = bytes_to_int_reverse_order(current_line[1:])
                src_code = src_code[:location]+int.to_bytes(regesters['E'], length = 1, byteorder = 'little')+src_code[location+1:]
            # I'll use brackets to mean [_] = Regester _ in the comments to not type as much from here on out...
            case 43: #+ # [A] + [B] = [C] ([E] = 1 for overflow, and 0 otherwise)
                regesters['C'] = regesters['A'] + regesters['B']
                regesters['E'] = 0
                while regesters['C'] > 255 :
                    regesters['C'] -= 256
                    regesters['E'] += 1
            case 45: #- # [A] - [B] = [C] ([E] = 255 for underflow, and 0 otherwise)
                regesters['C'] = regesters['A'] - regesters['B']
                regesters['E'] = 0
                while regesters['C'] < 0 :
                    regesters['C'] += 256
                    regesters['E'] -= 1
                while regesters['E'] < 0 :
                    regesters['E'] += 256
            case 38: #& # [A] & [B] = [C] ([E] = 0)
                regesters['C'] = regesters['A'] & regesters['B']
                regesters['E'] = 0
            case 124: #| # [A] | [B] = [C] ([E] = 0)
                regesters['C'] = regesters['A'] | regesters['B']
                regesters['E'] = 0
            case 94: #^ # [A] ^ [B] = [C] ([E] = 0)
                regesters['C'] = regesters['A'] ^ regesters['B']
                regesters['E'] = 0
            case 106: #j # Jump to the address
                current_line_position = bytes_to_int_reverse_order(current_line[1:]) - WIDTH
            case 62: #> # Jump to the address if [A] > [B]
                if regesters['A'] > regesters['B'] :
                    current_line_position = bytes_to_int_reverse_order(current_line[1:]) - WIDTH
            case 61: #= # Jump to the address if [A] == [B]
                if regesters['A'] == regesters['B'] :
                    current_line_position = bytes_to_int_reverse_order(current_line[1:]) - WIDTH
            case 60: #< # Jump to the address if [A] < [B]
                if regesters['A'] < regesters['B'] :
                    current_line_position = bytes_to_int_reverse_order(current_line[1:]) - WIDTH
            case 114: #r # Jump to the address and return to the next line later
                depth_stack.append(current_line_position+WIDTH)
                current_line_position = bytes_to_int_reverse_order(current_line[1:]) - WIDTH
            case 44: #, # Input char (Like BrainF***) (Store in [C])
                while len(character_input_que) == 0 :
                    character_input_que += input()
                regesters['E'] = 1 if len(character_input_que)==1 else 0 # [E] is one for end of input
                regesters['C'] = ord(character_input_que[0])
                character_input_que = character_input_que[1:]
            case 59: #; # Input unsigned 8-bit int (Store in [C])
                character_input_que = ''
                i = int(input())
                regesters['C'] = i%256
                regesters['E'] = 0 if i == regesters['C'] else 1
            case 46: #. # Output char (Like BrainF***) (Get from [A])
                print(chr(regesters['A']), end='')
            case 58: #: # Output unsigned 8-bit int (Get from [A])
                print(regesters['A'], end='')
            case 63: #? # Set [C] to a random number between [A] and [B] inclusive
                regesters['C'] = random.randint(regesters['A'], regesters['B'])
            case 0: #Nothing # Go back to the last thing in the depth_stack and pop that bad boi
                break # Do this via breaking out of the while True
            case _:
                # This should not occur. I'll treat it like a comment, I guess.
                pass
        current_line_position += WIDTH

input("\n\nPress [ENTER] to end program. ")
