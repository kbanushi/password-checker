# password-checker
This app will check a user's inputted password against a database of leaked passwords and common English words while maintaining O(LogN) time complexity.  Eventually I hope to make this app not assign each word to a vector element to try and save space, so I will instead read through the file using the seekg function.
