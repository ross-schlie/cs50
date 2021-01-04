import sys
import csv
import re

# The program should require as its first command-line argument the name of a CSV file
#  containing the STR counts for a list of individuals and should require as its
#  second command-line argument the name of a text file containing the DNA sequence to identify.
# If your program is executed with the incorrect number of command-line arguments,
#  your program should print an error message of your choice (with print).
# If the correct number of arguments are provided, you may assume that the first argument
#  is indeed the filename of a valid CSV file, and that the second argument is the filename of a valid text file.
if len(sys.argv) != 3:
    print("Usage: python dna.py [CSV FILE] [TEXT FILE]")
    sys.exit(1)

# Try to open csv
csvfilename = sys.argv[1]
csvfile = open(csvfilename, "r", encoding="latin_1")
if not csvfile:
    print("Could not open {}." . format(csvfilename))
    sys.exit(1)
csvfile.close()

# Try to open text
textfilename = sys.argv[2]
textfile = open(textfilename, "r", encoding="latin_1")
if not textfile:
    print("Could not open {}." . format(textfilename))
    sys.exit(1)

# Your program should open the CSV file and read its contents into memory.
# You may assume that the first row of the CSV file will be the column names.
# The first column will be the word name and the remaining columns will be the STR sequences themselves.
# Since we will be searching these anyway, it seems silly to load all the rows into memory and compare them later,
# so while technically they are being loaded into memory, i do so one at a time at time of comparison (later)
with open(csvfilename, newline='') as csvfile:
    reader = csv.DictReader(csvfile)
    columnNames = reader.fieldnames

# Your program should open the DNA sequence and read its contents into memory.
# For each of the STRs (from the first line of the CSV file),
#  your program should compute the longest run of consecutive repeats of the STR in the DNA sequence to identify.
sequences = {}
while True:
    #single line??
    textfileline = textfile.readline()
    if not textfileline:
        break

    # e.g.
    # sequences['AGATC'] = textfileline.count('AGATC')
    for i in range(len(columnNames)):
        # skip 'name'
        if i != 0:
            pattern = '((' + columnNames[i] + ')+)+'
            # pattern = columnNames[i]
            matches = re.findall(pattern, textfileline)
            longestMatch = 0
            if matches:
                # print(columnNames[i], pattern, match)
                for m in range(len(matches)):
                    match = matches.pop()
                    # print(match[0])
                    currentMatchLength = match[0].count(columnNames[i])
                    if currentMatchLength > longestMatch:
                        # print('new longest', match[0])
                        longestMatch = currentMatchLength

            # print(longestMatch)
            # sequences[columnNames[i]] = int(textfileline.count(columnNames[i]))
            sequences[columnNames[i]] = longestMatch

# print(sequences)

# Your program should open the CSV file and read its contents into memory.
# This is the second time we open it, but now we are actuallly checking it row by row (instead of in memory)
# If the STR counts match exactly with any of the individuals in the CSV file,
# your program should print out the name of the matching individual.
# You may assume that the STR counts will not match more than one individual.
foundMatch  = False
with open(csvfilename, newline='') as csvfile:
    reader = csv.DictReader(csvfile)
    for row in reader:
        # start as True, but if any of the STRs do not match, change to False
        rowMatches = True
        for i in range(len(columnNames)):
            # skip 'name'
            if i != 0:
                value = int(row[columnNames[i]])
                if sequences[columnNames[i]] != value:
                    # print(row[reader.fieldnames[0]], " mismatched ", columnNames[i], sequences[columnNames[i]], value, sequences[columnNames[i]] == value)
                    rowMatches = False
                    break
                # else:
                    # print(row[reader.fieldnames[0]], " matched ", columnNames[i], sequences[columnNames[i]], value, sequences[columnNames[i]] == value)

        if rowMatches:
            print(row[reader.fieldnames[0]])
            foundMatch = True
            # break

# If the STR counts do not match exactly with any of the individuals in the CSV file, your program should print "No match".
if foundMatch == False:
    print("No match")