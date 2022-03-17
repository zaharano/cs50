#include <stdio.h>
#include <cs50.h>

string processCC(long num);

int main(void)
{
    long cc = get_long("GIVE ME YOUR CREDIT CARD NUMBER!: ");

    string result = processCC(cc);

    printf("%s\n", result);
}

string processCC(long num)
{
    // init
    int length = 0,
        sixteen = 0,
        fifteen = 0,
        fourteen = 0,
        thirteen = 0,
        digit = 0,
        algTestTot = 0,
        algTestDigit = 0,
        algTestDigitDigit = 0;

    // while cc num isn't reduced to 0, break into digits for testing and assigning, then div by 10
    while (num)
    {
        length++;
        // pull the end digit with mod 10
        digit = num % 10;
        // if we've reached these digits, they are cached for the type of card checks
        if (length == 13)
        {
            thirteen = digit;
        }
        else if (length == 14)
        {
            fourteen = digit;
        }
        else if (length == 15)
        {
            fifteen = digit;
        }
        else if (length == 16)
        {
            sixteen = digit;
        }

        // if a second digit, multiply by two. add digits of product to sum
        if (length % 2 == 0)
        {
            algTestDigit = digit * 2;
            while (algTestDigit)
            {
                // process digits just like the main num
                algTestDigitDigit = algTestDigit % 10;
                algTestTot += algTestDigitDigit;
                algTestDigit /= 10;
            }
        }
        else
        {
            // add non second digits to sum
            algTestTot += digit;
        }
        // removed the processed digit from end
        num /= 10;
    }
    // test for Luhn's Algorithm, and that the length is within valid range
    if (algTestTot % 10 != 0 || length > 16 || length < 13)
    {
        return "INVALID";
    }
    // if those tests are passed, check lengths / values of first digits for typing / more validity
    else
    {
        if (length == 13)
        {
            if (thirteen == 4)
            {
                return "VISA";
            }
            else
            {
                return "INVALID";
            }
        }
        else if (length == 14)
        {
            return "INVALID";
        }
        else if (length == 15)
        {
            if ((fifteen == 3) && (fourteen == 4 || fourteen == 7))
            {
                return "AMEX";
            }
            else
            {
                return "INVALID";
            }
        }
        else if (length == 16)
        {
            if (sixteen == 5 && fifteen > 0 && fifteen < 6)
            {
                return "MASTERCARD";
            }
            else if (sixteen == 4)
            {
                return "VISA";
            }
            else
            {
                return "INVALID";
            }
        }
    }
    return "INVALID";
}