import cs50


def processCC(cc):
    length = len(cc)
    LuhnTestTot = 0

    if length > 16 or length < 13:
        return "INVALID"

    for i in range(1, length + 1):
        intDigit = int(cc[length - i])
        if i % 2 == 0:
            LuhnTest = str(intDigit * 2)
            for digit in LuhnTest:
                LuhnTestTot += int(digit)
        else:
            LuhnTestTot += intDigit

    if LuhnTestTot % 10 != 0:
        return "INVALID luhn"

    if length == 13:
        if cc[0] == "4":
            return "VISA"
        else:
            return "INVALID"
    elif length == 14:
        return "INVALID"
    elif length == 15:
        if (cc[0] == "3") and (cc[1] == "4" or cc[1] == "7"):
            return "AMEX"
        else:
            return "INVALID"
    elif length == 16:
        if (cc[0] == "5") and int(cc[1]) > 0 and int(cc[1]) < 6:
            return "MASTERCARD"
        elif cc[0] == "4":
            return "VISA"
        else:
            return "INVALID"
    return "you broke it"


cc = cs50.get_string("Gimme your cc number: ")
result = processCC(cc)
print(result)
