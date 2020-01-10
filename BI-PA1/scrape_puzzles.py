import urllib.request
import sys
import re
import time
from bs4 import BeautifulSoup
import subprocess
import numpy as np
from random import randint


numpuzzles = 10000
cntup = 0
timeout = numpuzzles
print ('Progress:')
their_w = 9
their_h = 17
diff = "expert"
real_w = their_w + 1
real_h = their_h + 1
while cntup < timeout:
    with open("savedpuzzles.txt", "w") as file:
        kakuropage = urllib.request.urlopen('http://www.kakuroconquest.com/' + str(their_w) + 'x' + str(their_h) +'/' + diff)
        # kakuropage = urllib.request.urlopen('http://www.kakuroconquest.com/')
        time.sleep(2)
        kakurosoup = BeautifulSoup(kakuropage,'html.parser')
        puzzleid = 0
        for souptitle in kakurosoup.find_all(class_="text1"):
            content = souptitle.text
            if (content[1]=='N'):
                puzzlenumber = re.search(r'\d+$', content.strip()).group()
                puzzleid = int(puzzlenumber)
        else:
            cntup = cntup + 1
            numeq = (cntup*10)/timeout
            sys.stdout.write('\r')
            sys.stdout.write("[%-10s] %f%%" % ('=' * int(numeq), float(cntup*100)/timeout))
            sys.stdout.flush()
        board = np.zeros((real_h, real_w, 3))
        for elem in kakurosoup.find_all(class_="cellTotal"):
            for elemitem in elem.find_all('input'):
                addr = elemitem['name']
                total = int(elemitem['value'])
                if addr[9] == "_":
                    y = int(addr[7] + addr[8])
                    x = int(addr[10])
                else:
                    y = int(addr[7])
                    x = int(addr[9])
                if str(addr[5]) == "v":
                    board[y][x][1] = total;
                    board[y][x][0] = 1;
                if str(addr[5]) == "h":
                    board[y][x][2] = total;
                    board[y][x][0] = 1;
        for elem in kakurosoup.find_all(class_="cellNumber"):
            for elemitem in elem.find_all('input'):
                addr = elemitem['name']
                if addr[7] == "_":
                    y = int(addr[5] + addr[6])
                    x = int(addr[8])
                else:
                    y = int(addr[5])
                    x = int(addr[7])
                board[y][x][0] = -1
        for y in range(real_h):
            for x in range(real_w):
                if board[y][x][0] == 0:
                    file.write("X")
                if board[y][x][0] == -1:
                    file.write(".")
                if board[y][x][0] == 1:
                    if board[y][x][1] == 0:
                        file.write("X")
                    else:
                        file.write(str(int(board[y][x][1])))
                    file.write("\\")
                    if board[y][x][2] == 0:
                        file.write("X")
                    else:
                        file.write(str(int(board[y][x][2])))
                if x != real_w - 1:
                    file.write("   ")
            file.write("\n")
    p = subprocess.run("cp savedpuzzles.txt CZE/" + str(randint(1, 99999999)), shell=True)
    pr = subprocess.run('./autom.out < savedpuzzles.txt > /dev/null', shell=True)
    if pr.returncode != 0:
        print("Ladies and gentlemen, we got him")
        break
