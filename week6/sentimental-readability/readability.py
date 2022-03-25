import cs50

text = cs50.get_string("What're we testing?: ")

letter_count = 0
word_count = 0
sentence_count = 0
word_started = sentence_started = False

for character in text:
    if character.isalpha():
        word_started = True
        sentence_started = True
        letter_count += 1
    elif character == " " and word_started:
        word_count += 1
        word_started = False
    elif character == "." or character == "?" or character == "!":
        if word_started:
            word_count += 1
            word_started = False
        if sentence_started:
            sentence_count += 1
            sentence_started = False

if word_count == 0:
    print("Text must have at least one terminated word!")
    exit()

lpw = letter_count / word_count
spw = sentence_count / word_count

index = round(0.0588 * (lpw * 100) - 0.296 * (spw * 100) - 15.8)

if index < 1:
    print("Before Grade 1")
elif index > 15:
    print("Grade 16+")
else:
    print(f"Grade {index}")
