import os
import sys
import argparse
import hashlib

parser = argparse.ArgumentParser(prog = 'check integrity', description = 'recursive check path or file integrity')
parser.add_argument('--path', default='.', help='specify a path to 1. init merkle tree node alongside every path(file). or 2. check integrity by nodes already exist')
parser.add_argument('--hash_type', default='sha256', choices=["sha256", "sha512"], help='hash_type in hashlib')
parser.add_argument('--hash_code', required=True)
args = parser.parse_args()

def merkle(target):
    if os.path.isfile(target):
        BLOCK_SIZE = 65536
        file_hash = getattr(hashlib, args.hash_type)()
        with open(target, 'rb') as f:
            fb = f.read(BLOCK_SIZE)
            while len(fb) > 0:
                    file_hash.update(fb)
                    fb = f.read(BLOCK_SIZE)
            return file_hash.hexdigest()
    elif os.path.isdir(target):
        hash_content = target
        for i in os.listdir(target):
            hash_content += merkle(os.path.join(target, i))
        return getattr(hashlib, args.hash_type)(hash_content.encode('utf-8')).hexdigest()


m = merkle(args.path)
if m == args.hash_code:
    print("Good")
    sys.exit(0)
else:
    print(f"Bad, {m}")
    sys.exit(1)
