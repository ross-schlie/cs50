from cs50 import get_string

def main():
    userText = get_string("Text: ")

    # letterCount = count_letters(userText)
    # print(f"{letterCount} letter(s)")

    # wordCount = count_words(userText)
    # print(f"{wordCount} word(s)")

    # sentenceCount = count_sentences(userText)
    # print(f"{sentenceCount}} sentence(s)")

    # index = calc_index(letterCount, wordCount, sentenceCount)
    index = calc_index_efficient(userText)

    # outputs the grade level as given by the Coleman-Liau index (e.g. "Grade 2" or "Grade 8").
    # Be sure to round the resulting index number to the nearest whole number!
    # If the resulting index number is 16 or higher (equivalent to or greater than a senior undergraduate reading level),
    # your program should output "Grade 16+" instead of giving the exact index number.
    # If the index number is less than 1, your program should output "Before Grade 1".
    if index > 16:
        print("Grade 16+")
    elif index < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {index}")

# Treat a string as an array and check each character to see if it's alphabetical or not. return the count of alphabeticals
def count_letters(text):
    letters = 0
    length = len(text)
    for i in range(length):
        if (text[i].isalpha()):
            letters += 1

    return letters

# Treat a string as array and count "any sequence of characters separated by a space to be a word"
def count_words(text):
    length = len(text)
    words = 0
    for i in range(length):
        # this will be wrong if there's an double space.
        # if (isspace(text[i]))
        if text[i].isspace() and text[i - 1].isspace() != True:
            words += 1

    #check to see if the very last letter is a space or not. if not, increment by one (since the last word should also count.)
    if text[length].isspace() != True:
        words += 1

    return words

# You might first imagine that a sentence is just any sequence of characters that ends with a period,
#  but of course sentences could end with an exclamation point or a question mark as well.
# But of course, not all periods necessarily mean the sentence is over.
# we’ll ask you to ignore that subtlety: you should consider any sequence of characters that ends with a . or a ! or a ? to be a sentence
def count_sentences(text):
    length = len(text)
    sentences = 0

    for i in range(length):
        #ignoring odd cases of double punctuations
        #ispunct does not only specify .!?

        if (text[i] == '.' or text[i] == '?' or text[i] == '!'):
            sentences += 1

    return sentences

#index = 0.0588 * L - 0.296 * S - 15.8
#where L is the average number of letters per 100 words in the text, and S is the average number of sentences per 100 words in the text.
def calc_index(letterCount, wordCount, sentenceCount):
    perHundred = wordCount / 100
    L = letterCount / perHundred
    S = sentenceCount / perHundred
    index = 0.0588 * L - 0.296 * S - 15.8
    # print(f"{letterCount} letter(s)")
    # print(f"{wordCount} word(s)")
    # print(f"{sentenceCount} sentence(s)")
    # print(f"{perHundred} per 100  ")
    # print(f"{L} L")
    # print(f"{S} S")
    # print(f"{index} index")

    return round(index)

# instead of 3N we do N (single loop!)
def calc_index_efficient(text):
    length = len(text)
    letters = 0
    words = 0
    sentences = 0
    for i in range(length):
        if text[i].isalpha():
            letters += 1

        if text[i].isspace() and text[i - 1].isspace() != True:
            words += 1

        if (text[i] == '.' or text[i] == '?' or text[i] == '!'):
            sentences += 1

    #check to see if the very last letter is a space or not. if not, increment by one (since the last word should also count.)
    if text[length - 1].isspace() != True:
        words += 1

    # print(f"{letters} letter(s)")
    # print(f"{words} word(s)")
    # print(f"{sentences} sentence(s)")

    return calc_index(letters, words, sentences)

main()