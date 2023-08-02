
# gets credit card number
cc_num = int(input("Number: "))
cc_num_copy = cc_num
sum = 0
# print(cc_num)
num_digits = 0

while (cc_num > 0):
    # counts by two or one since ccnum is divided by 10 twice below
    if (cc_num > 9):
        num_digits = num_digits + 2
    else:
        num_digits = num_digits + 1

    sum = sum + (cc_num % 10)
    cc_num = cc_num // 10
    dummy = (cc_num % 10) * 2
    if (dummy < 10):  # if dummy is double digits, we need to split it into single digits
        sum = sum + dummy
    else:
     # splits into single digits
        sum = sum + (dummy % 10)
        dummy = dummy // 10
        sum = sum + dummy

    if (cc_num > 100 and cc_num < 1000):
        first_two = cc_num // 10  # /* first two digits of cc_num */
    elif (cc_num > 9):
        first_two = cc_num
    cc_num = cc_num // 10
    # print(cc_num)

# automatically determines if invalid based on sum
if (sum % 10 != 0):
    print("INVALID")

# first two is first two digits; numdigits is number of digits
elif (first_two >= 51 and first_two <= 55 and num_digits == 16):
    print("MASTERCARD")
elif ((first_two == 34 or first_two == 37) and num_digits == 15):
    print("AMEX")
elif (first_two >= 40 and first_two <= 49 and (num_digits == 13 or num_digits == 16)):
    print("VISA")
else:
    print("INVALID")
