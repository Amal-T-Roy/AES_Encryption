import key_generator
import unittest
import os

class test_key_generator(unittest.TestCase):
    def setup(self):
        pass

    # Method to test if file is being written into
    def test_GenerateKey(self):
        # create temp file
        with open("key.txt", "w") as f:
            pass

        # call function
        key_generator.GenerateKey()

        # Verify that the file was written to correctly
        with open("key.txt", "r") as f:
            data = f.read()
        self.assertIsNotNone(data)

    # Method to test if file is being overwritten
    def test_FileEditing(self):
        # read from temp file
        with open("key.txt", "r") as f:
            OriginalFileContent = f.readlines()

        # Call the function to generate key,which modifies the data in file
        key_generator.GenerateKey()

        # Read new content
        with open("key.txt", "r") as f:
            NewContent = f.readlines()

        # Compare them
        self.assertNotEqual(OriginalFileContent, NewContent)

    # Method to check if file is created,when file doesn't exist
    def test_FileCreation(self):
        # Delete existing file
        os.remove("key.txt")

        #Check if file is deleted
        self.assertFalse(os.path.exists("key.txt"))


        #Call function,which should result in creation of .txt file
        key_generator.GenerateKey()

        #Check if file is created
        self.assertTrue(os.path.exists("key.txt"))


if __name__ == "__main__":
    unittest.main()
