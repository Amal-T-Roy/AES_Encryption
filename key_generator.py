#import secrets
import random

def GenerateKey():
    key = random.getrandbits(64)
    with open('key.txt','w') as f:
        f.write(str(key))
    return key

print('****Python****')
Key = (GenerateKey()) # Random Key is generated
print(type(Key))
print(Key) # Prints the key in the terminal
print('****END****\n')
