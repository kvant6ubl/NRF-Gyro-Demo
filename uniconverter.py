import uuid
import random
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--uuidN', help='Input the number of <string> UUID ti generate')
args = parser.parse_args()

number_of_uuid = int(args.uuidN)

for uu in range(0, number_of_uuid):
    temp_usr_uuid = uuid.uuid3(uuid.NAMESPACE_DNS, str(random.randint(-1000 , 1000)))
    print("Take your " + str(uu) + " UUID: " + str(temp_usr_uuid))

"""
    UUID Class Instance examples:

    1. make a UUID based on the host ID and current time
    >>> usr_uuid = uuid.uuid1()

    2. make a UUID using an MD5 hash of a namespace UUID and random ASCII value
    >>> usr_uuid = uuid.uuid3(uuid.NAMESPACE_DNS, str(random.randint(-1000 , 1000)))

    3. convert a UUID to a string of hex digits in standard form
    >>> x = uuid.UUID('{00010203-0405-0607-0809-0a0b0c0d0e0f}')
    >>> str(x)
    >>> '00010203-0405-0607-0809-0a0b0c0d0e0f'
"""