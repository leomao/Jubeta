#! /usr/bin/env python

import os
import glob

def convert_sheet():
    if not os.path.isdir("convert"):
        return False

    for filename in glob.iglob(os.path.join("convert", "*.txt")):
        print("converting", filename, "...")
        try:
            memo = open(filename, "r")
            jub = open(os.path.splitext(filename)[0] + ".jub", "w")
        except:
            print("failed.")
            continue
        
        line = memo.readline()
        while line:
            line = line.strip()
            if line == "#memo" or line == "#memo1":
                print("#start#", file=jub)
                ismemo = True
                break
            elif line == "#numbersheet":
                print("#start#", file=jub)
                ismemo = False
                break
            else:
                print(line, file=jub)
            line = memo.readline()
        
        line = memo.readline()
        now = 0
        while line:
            line = line.strip()
            if not line:
                pass
            elif (ismemo and ord(line[0]) <= ord('9') and
                ord(line[0]) >= ord('0')):
                pass
            elif line[0] == 't':
                print(round(now), line, file=jub)
            else:
                notes = []
                beats = []
                try:
                    note, beat = line.split('|')[:2]
                    notes.append(note.strip())
                    beats.append(beat)
                except ValueError:
                    notes.append(line)
                count = 1
                line = memo.readline()
                while line:
                    line = line.strip()
                    try:
                        note, beat = line.split('|')[:2]
                        notes.append(note.strip())
                        beats.append(beat)
                    except ValueError:
                        notes.append(line)
                    count += 1
                    if count == 4:
                        break
                    line = memo.readline()
                jb = {}
                notes = ''.join(notes)
                for i, char in enumerate(notes):
                    if char != '□' and char != ' ' and char != '0':
                        jb[char] = jb.get(char, 0) + (1 << i)
                for beat in beats:
                    tmp_itv = 1000 / len(beat)
                    for char in beat:
                        if char in jb:
                            print(round(now), jb[char], file=jub)
                        now += tmp_itv
            line = memo.readline()
        memo.close()
        print("done.")

if __name__ == "__main__":
    convert_sheet()

