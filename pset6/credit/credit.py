from cs50 import get_int

def main():
    valid = False

    # ask the user the cc number
    ccnum = get_int("Number: ")
    cclength = len(str(ccnum))

    if cclength > 12 and cclength < 17:
        # check checksum
        valid = checkCC(ccnum, cclength)

    if valid:
        # use length and starting nums to decide type
        if isVisa(ccnum, cclength):
            print("VISA")
        elif isAmex(ccnum, cclength):
            print("AMEX")
        elif isMasterC(ccnum, cclength):
            print("MASTERCARD")
        else:
            print("INVALID")
    else:
        print("INVALID")

# using modular, get every digit; use Luhn’s Algorithm
# Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products’ digits together.
# Add the sum to the sum of the digits that weren’t multiplied by 2.
# If the total’s last digit is 0 (or, put more formally, if the total modulo 10 is congruent to 0), the number is valid!
def checkCC(ccnum, cclength):
    tenPow = 10
    modulus = 0
    ccnumMinusMod = ccnum

    oddSum = 0
    evenSum = 0
    product = 0
    productMod = 0

    for i in range(cclength):
        modulus = ccnumMinusMod % tenPow
        # print(f"DEBUG DIGIT modulus {ccnumMinusMod} mod {tenPow} = {modulus}")
        ccnumMinusMod -= modulus
        ccnumMinusMod = ccnumMinusMod / tenPow
        # print(f"DEBUG DIGIT cc minus modulus and dived by pow = {ccnumMinusMod}")

        # print(f"DEBUG i mod 2 = {i % 2}")
        if i % 2 == 0:
            # digits that weren’t multiplied by 2 (odd digits)
            evenSum += modulus
        else:
            # add those products’ digits together
            product = modulus * 2
            productMod = product % 10
            # print(f"DEBUG product div 10 { product / 10} and product mod 10  {productMod}")
            if product / 10 >= 1:
                oddSum += 1
                oddSum += productMod
            else:
                oddSum += product

            # print(f"DEBUG modulus = {modulus}")
            # print(f"DEBUG product = {product}")
            # print(f"DEBUG odd SUM = {oddSum}")

    # print(f"DEBUG final odd SUM = {oddSum}")
    # print(f"DEBUG final even SUM = {evenSum}")

    sum = evenSum + oddSum
    # print(f"DEBUG SUM mod 10 = {sum % 10}")
    if sum % 10 == 0:
        # print("VALID CHECKSUM")
        return True

    return False

# American Express uses 15-digit numbers
# All American Express numbers start with 34 or 37
def isAmex(ccnum, cclength):
    if cclength == 15:
        # non-array version
        firstDigit = ccnum / 100000000000000
        secondDigit = (ccnum / 10000000000000) % 10

        # debug info
        # print(f"AMEX length, digits = {firstDigit} , {secondDigit}")
        if (firstDigit >= 3 and firstDigit < 4) and ((secondDigit >= 4 and secondDigit < 5) or (secondDigit >= 7 and secondDigit < 8)):
            return True

    return False

# most MasterCard numbers start with 51, 52, 53, 54, or 55
# MasterCard uses 16-digit numbers
def isMasterC(ccnum, cclength):
    if cclength == 16:
        # non-array version
        firstDigit = ccnum / 1000000000000000
        secondDigit = (ccnum / 100000000000000) % 10

        # debug info
        # print(f"MASTERC length, digits = {firstDigit} , {secondDigit}")
        if (firstDigit >= 5 and firstDigit < 6) and (secondDigit >= 1 and secondDigit < 6):
            return True

    return False

# and all Visa numbers start with 4
# Visa uses 13- and 16-digit numbers
def isVisa(ccnum, cclength):
    if cclength == 16 or cclength == 13:
        firstDigit = 0
        # non-array version
        if cclength == 16:
            firstDigit = ccnum / 1000000000000000
        elif cclength == 13:
            firstDigit = ccnum / 1000000000000

        # debug info
        # print(f"VISA length, first digit = {firstDigit}")
        if firstDigit >= 4 and firstDigit < 5:
            return True

    return False

main()