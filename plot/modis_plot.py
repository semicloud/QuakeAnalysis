# MODIS plot script
import argparse

parser = argparse.ArgumentParser(description='MODIS plot script, Version:')
parser.add_argument('-y', '--yml', help='yml配置文件', required=True)

args = parser.parse_args()

if args.yml:
    print("yml load:" + args.yml)
