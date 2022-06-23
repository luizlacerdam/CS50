# TODO
from cs50 import get_string


text = get_string("What's the text? ")

# Count Letters
letters = 0
for x in text:
    if (x.isalpha() == True):
        letters += 1

# Count words
words = 1
for x in text:
    if (x.isspace() == True):
        words += 1

# Count setences
setences = 0
for x in text:
    if (x in ["!", ".", "?"]):
        setences += 1

L = (letters * 100) / words

S = (setences * 100) / words

grade = round((0.0588 * L) - (0.296 * S) - 15.8)

if grade >= 16:
    print("Grade 16+")
elif grade < 1:
    print("Before Grade 1")
else:
    print(f"Grade {grade}")
