using LR2;

long p = 17;
long q = 19; 

var rsa = new RSAEncryption(p, q);

string message = "Information";
Console.WriteLine("Message: " + message);

string encryptedMessage = rsa.Encrypt(message);
Console.WriteLine("Encrypted Message: " + encryptedMessage);

string decryptedMessage = rsa.Decrypt(encryptedMessage);
Console.WriteLine("Decrypted Message: " + decryptedMessage);