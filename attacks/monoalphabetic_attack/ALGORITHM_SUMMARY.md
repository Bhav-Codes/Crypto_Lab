# Algorithm Summary for Monoalphabetic Cipher Cryptanalysis

## 1. Encryption Algorithm

### Monoalphabetic Substitution Cipher
```
Algorithm: MONOALPHABETIC_ENCRYPT(plaintext, key)
Input: plaintext (string), key (26-letter substitution array)
Output: ciphertext (string)

1. Initialize empty ciphertext string
2. FOR each character c in plaintext DO:
   3.  IF c is alphabetic THEN:
   4.     isUpper ← (c is uppercase)
   5.     index ← position of c in alphabet (0-25)
   6.     substituted ← key[index]
   7.     IF NOT isUpper THEN:
   8.        substituted ← lowercase(substituted)
   9.     Append substituted to ciphertext
   10. ELSE:
   11.    Append c to ciphertext (keep non-letters unchanged)
12. RETURN ciphertext
```

**Time Complexity**: O(n) where n is the length of plaintext  
**Space Complexity**: O(n) for output ciphertext

---

## 2. Frequency Analysis Algorithm

### Letter Frequency Counter
```
Algorithm: FREQUENCY_ANALYSIS(ciphertext)
Input: ciphertext (string)
Output: list of (letter, count, percentage) sorted by frequency

1. Initialize frequency array freq[26] = {0}
2. Initialize totalLetters ← 0
3. 
4. // Count letter frequencies
5. FOR each character c in ciphertext DO:
6.    IF c is alphabetic THEN:
7.       upper ← uppercase(c)
8.       index ← (upper - 'A')
9.       freq[index] ← freq[index] + 1
10.      totalLetters ← totalLetters + 1
11.
12. // Calculate percentages
13. Initialize result array
14. FOR i = 0 TO 25 DO:
15.    IF freq[i] > 0 THEN:
16.       letter ← ('A' + i)
17.       count ← freq[i]
18.       percentage ← (count × 100.0) / totalLetters
19.       Add (letter, count, percentage) to result
20.
21. // Sort by frequency (descending)
22. SORT result by count in descending order
23.
24. // Display results with English comparison
25. FOR each (letter, count, percentage) in result DO:
26.    englishFreq ← expected English frequency for letter
27.    PRINT letter, count, percentage, englishFreq
28.
29. RETURN result
```

**Time Complexity**: O(n + k log k) where n = text length, k = unique letters (max 26)  
**Space Complexity**: O(k) for frequency storage

---

## 3. Word Frequency Analysis Algorithm

### Extract and Analyze Words
```
Algorithm: WORD_FREQUENCY_ANALYSIS(ciphertext)
Input: ciphertext (string)
Output: map of word_length → list of frequent words

1. Initialize wordsByLength map (length → word → count)
2. Initialize currentWord ← empty string
3.
4. // Extract words from ciphertext
5. FOR each character c in ciphertext DO:
6.    IF c is alphabetic THEN:
7.       currentWord ← currentWord + uppercase(c)
8.    ELSE:
9.       IF currentWord is not empty THEN:
10.         length ← length of currentWord
11.         wordsByLength[length][currentWord] ← 
12.            wordsByLength[length][currentWord] + 1
13.         currentWord ← empty string
14.
15. // Process last word if any
16. IF currentWord is not empty THEN:
17.    length ← length of currentWord
18.    wordsByLength[length][currentWord] ← 
19.       wordsByLength[length][currentWord] + 1
20.
21. // Analyze and display by word length
22. FOR length = 1 TO 3 DO:
23.    IF wordsByLength contains length THEN:
24.       words ← all words of this length
25.       SORT words by frequency (descending)
26.       
27.       PRINT length + "-letter words:"
28.       FOR each word in top 20 words DO:
29.          PRINT word and its frequency
30.
31. // Display repeated longer words (4+ letters)
32. FOR length ≥ 4 DO:
33.    FOR each word of this length DO:
34.       IF word appears ≥ 2 times THEN:
35.          Add (word, frequency) to repeated list
36.
37. SORT repeated list by frequency (descending)
38. Display top repeated words
39.
40. RETURN wordsByLength
```

**Time Complexity**: O(n + w log w) where n = text length, w = unique words  
**Space Complexity**: O(w) for word storage

---

## 4. Pattern Analysis Algorithm

### Word Pattern Matching
```
Algorithm: PATTERN_ANALYSIS(ciphertext)
Input: ciphertext (string)
Output: list of (word, pattern, frequency)

1. Initialize patterns map (pattern → (word, frequency))
2. Initialize currentWord ← empty string
3.
4. // Extract words and compute patterns
5. FOR each character c in ciphertext DO:
6.    IF c is alphabetic THEN:
7.       currentWord ← currentWord + uppercase(c)
8.    ELSE:
9.       IF currentWord is not empty THEN:
10.         pattern ← GET_WORD_PATTERN(currentWord)
11.         IF pattern in patterns THEN:
12.            patterns[pattern].frequency ← 
13.               patterns[pattern].frequency + 1
14.         ELSE:
15.            patterns[pattern] ← (currentWord, 1)
16.         currentWord ← empty string
17.
18. // Convert to list and sort
19. Initialize result list
20. FOR each pattern in patterns DO:
21.    Add (word, pattern, frequency) to result
22.
23. SORT result by (frequency DESC, word length DESC)
24.
25. // Display repeated patterns
26. FOR each entry in result DO:
27.    IF frequency ≥ 2 AND word length ≥ 3 THEN:
28.       PRINT pattern, word, frequency
29.
30. RETURN result

// Helper function to generate word pattern
Function GET_WORD_PATTERN(word)
Input: word (string)
Output: pattern (string of digits)

1. Initialize charMap (character → digit)
2. Initialize pattern ← empty string
3. nextDigit ← '1'
4.
5. FOR each character c in word DO:
6.    IF c NOT in charMap THEN:
7.       charMap[c] ← nextDigit
8.       nextDigit ← nextDigit + 1
9.    pattern ← pattern + charMap[c]
10.
11. RETURN pattern

Example: HELLO → 12334 (H=1, E=2, L=3, O=4)
```

**Time Complexity**: O(n × m) where n = text length, m = average word length  
**Space Complexity**: O(w) where w = unique words

---

## 5. Apply Substitution Algorithm

### Maintain Consistent Mapping
```
Algorithm: APPLY_SUBSTITUTION(cipherChar, plainChar)
Input: cipherChar (cipher letter), plainChar (plain letter)
Output: success (boolean)

1. cipherChar ← uppercase(cipherChar)
2. plainChar ← uppercase(plainChar)
3.
4. // Check for conflicts with existing mappings
5. IF cipherChar in currentSubstitution THEN:
6.    IF currentSubstitution[cipherChar] ≠ plainChar THEN:
7.       PRINT "Conflict: cipherChar already mapped differently"
8.       RETURN false
9.
10. IF plainChar in reverseSubstitution THEN:
11.    IF reverseSubstitution[plainChar] ≠ cipherChar THEN:
12.       PRINT "Conflict: plainChar already used"
13.       RETURN false
14.
15. // Apply new substitution
16. currentSubstitution[cipherChar] ← plainChar
17. reverseSubstitution[plainChar] ← cipherChar
18.
19. // Update partial plaintext
20. FOR i = 0 TO length(ciphertext) - 1 DO:
21.    IF ciphertext[i] is alphabetic THEN:
22.       upper ← uppercase(ciphertext[i])
23.       IF upper in currentSubstitution THEN:
24.          substituted ← currentSubstitution[upper]
25.          IF ciphertext[i] is uppercase THEN:
26.             partialPlaintext[i] ← substituted
27.          ELSE:
28.             partialPlaintext[i] ← lowercase(substituted)
29.
30. PRINT "Applied: cipherChar → plainChar"
31. RETURN true
```

**Time Complexity**: O(n) where n = ciphertext length  
**Space Complexity**: O(1) additional (updates existing structures)

---

## 6. Display Partial Plaintext Algorithm

### Show Decryption Progress
```
Algorithm: DISPLAY_PARTIAL_PLAINTEXT()
Input: (uses internal state: ciphertext, partialPlaintext, currentSubstitution)
Output: (displays to console)

1. PRINT "========== PARTIAL PLAINTEXT =========="
2.
3. // Display sample of text
4. PRINT "Ciphertext: " + first 80 characters of ciphertext
5. PRINT "Plaintext:  " + first 80 characters of partialPlaintext
6.
7. // Calculate progress
8. solved ← 0
9. total ← 0
10.
11. FOR each character c in ciphertext DO:
12.    IF c is alphabetic THEN:
13.       total ← total + 1
14.       upper ← uppercase(c)
15.       IF upper in currentSubstitution THEN:
16.          solved ← solved + 1
17.
18. // Display progress percentage
19. IF total > 0 THEN:
20.    progress ← (solved × 100.0) / total
21. ELSE:
22.    progress ← 0
23.
24. PRINT "Progress: solved/total letters (progress%)"
```

**Time Complexity**: O(n) where n = ciphertext length  
**Space Complexity**: O(1)

---

## 7. Verify Solution Algorithm

### Validate Recovered Key
```
Algorithm: VERIFY_SOLUTION(originalPlaintext)
Input: originalPlaintext (string)
Output: success (boolean)

1. // Extract only letters for comparison
2. original ← EXTRACT_LETTERS_ONLY(originalPlaintext)
3. decrypted ← EXTRACT_LETTERS_ONLY(partialPlaintext)
4.
5. // Check length match
6. IF length(original) ≠ length(decrypted) THEN:
7.    PRINT "Length mismatch"
8.    RETURN false
9.
10. // Count matches
11. matches ← 0
12. FOR i = 0 TO length(original) - 1 DO:
13.    IF original[i] = decrypted[i] THEN:
14.       matches ← matches + 1
15.
16. // Calculate accuracy
17. accuracy ← (matches × 100.0) / length(original)
18.
19. // Display results
20. PRINT "========== VERIFICATION RESULTS =========="
21. PRINT "Matches: matches/length (accuracy%)"
22.
23. // Determine success
24. IF accuracy ≥ 99.5 THEN:
25.    PRINT "SUCCESS! Plaintext recovered!"
26.    RETURN true
27. ELSE:
28.    PRINT "Partial solution. Continue analysis."
29.    RETURN false

Function EXTRACT_LETTERS_ONLY(text)
Input: text (string)
Output: letters (string)

1. letters ← empty string
2. FOR each character c in text DO:
3.    IF c is alphabetic THEN:
4.       letters ← letters + uppercase(c)
5. RETURN letters
```

**Time Complexity**: O(n) where n = text length  
**Space Complexity**: O(n) for extracted letter strings

---

## Complexity Summary

| Algorithm | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| Encryption | O(n) | O(n) |
| Frequency Analysis | O(n + k log k) | O(k) |
| Word Frequency | O(n + w log w) | O(w) |
| Pattern Analysis | O(n × m) | O(w) |
| Apply Substitution | O(n) | O(1) |
| Display Partial | O(n) | O(1) |
| Verify Solution | O(n) | O(n) |

Where:
- n = length of text
- k = number of unique letters (max 26)
- w = number of unique words
- m = average word length

---

## Complete Cryptanalysis Workflow

```
Algorithm: COMPLETE_CRYPTANALYSIS(ciphertext, originalPlaintext)
Input: ciphertext (string), originalPlaintext (optional, for verification)
Output: recovered plaintext (string)

1. // Phase 1: Initial Analysis
2. letterFreq ← FREQUENCY_ANALYSIS(ciphertext)
3. wordFreq ← WORD_FREQUENCY_ANALYSIS(ciphertext)
4. patterns ← PATTERN_ANALYSIS(ciphertext)
5.
6. // Phase 2: Interactive Decryption
7. WHILE solution not complete DO:
8.    DISPLAY_PARTIAL_PLAINTEXT()
9.    
10.   // Analyst makes hypothesis
11.   cipherChar ← GET_USER_INPUT("Cipher letter")
12.   plainChar ← GET_USER_INPUT("Plain letter")
13.   
14.   // Try substitution
15.   success ← APPLY_SUBSTITUTION(cipherChar, plainChar)
16.   
17.   IF NOT success THEN:
18.      PRINT "Rejected - conflict detected"
19.      option ← GET_USER_INPUT("Remove conflicting mapping? (y/n)")
20.      IF option = 'y' THEN:
21.         REMOVE_SUBSTITUTION(cipherChar)
22.         APPLY_SUBSTITUTION(cipherChar, plainChar)
23.   
24.   // Check if complete
25.   IF all letters mapped THEN:
26.      BREAK
27.
28. // Phase 3: Verification
29. IF originalPlaintext provided THEN:
30.    success ← VERIFY_SOLUTION(originalPlaintext)
31.    IF success THEN:
32.       PRINT "Cryptanalysis successful!"
33.    ELSE:
34.       PRINT "Review mappings - errors detected"
35.
36. RETURN partialPlaintext
```

This workflow combines all algorithms into a complete cryptanalysis process that can break monoalphabetic substitution ciphers through systematic analysis and iterative hypothesis testing.
