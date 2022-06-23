# TODO
from cs50 import get_int

while True:
    cardnumber = get_int("What's the card number? ")
    if cardnumber > 0:
        break
cardnumberStr = str(cardnumber)
cardnumberList = [cardnumberStr]
cardigits = len(cardnumberStr)

# Make a copy to manipulate
copyCardnumberStr = cardnumberStr[::-1]

# Othe digits to sum and creat a list of it
otherdigits = sum([int(x) for x in copyCardnumberStr[0::2]])

# Multiplies everyother by two and creat a list
everyothersum = sum([((int(x) * 2) // 10) + ((int(x) * 2) % 10) for x in copyCardnumberStr[1::2]])

sum = everyothersum + otherdigits

# Status = 1 INVALID card
status = 1

if sum % 10 != 0:
    print("INVALID")

else:
    status = 0

    if (cardigits == 15) and (cardnumberStr[0:2] in ["34", "37"]):
        print("AMEX")
    elif (cardigits == 16) and (cardnumberStr[0:2] in ["51", "52", "53", "54", "55"]):
        print("MASTERCARD")
    elif (cardigits in [13, 16]) and (cardnumberStr[0] == "4"):
        print("VISA")