from os import listdir
from os.path import isfile, join
import os.path

if __name__ == '__main__':
    if os.path.isfile('all_download.txt'):
        os.remove('all_download.txt')
        print('Already exist all_download.txt, delete it')

    txt_files = [f for f in listdir('.') if isfile(join('.', f)) and f.endswith(".txt")]
    download_urls = []
    for txt_file in txt_files:
        q = [line.rstrip('\n') for line in open(txt_file)]
        download_urls.extend(q)
        print('%s --> %d urls..' % (txt_file, len(q)))
    
    print("totally, %d urls loaded.." % len(download_urls))
    
    with open('all_download.txt','w') as f:
        for download_url in download_urls:
            f.write("%s\n" % download_url)

    print('all download urls saved into all_download.txt..')

    with open('download.bat','w') as f:
        f.write(u"aria2c.exe -i all_download.txt -d F:\\xx -l d.log --log-level notice --file-allocation=none")

    print('now, run download.bat to download data..')