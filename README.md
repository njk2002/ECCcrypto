# ECCcrypto

Using libsodium.

Goals
1.Implement chacha20 text encryption/decryption.
2.Implement X25519 text encryption/decryption.
3.Implement file encryption/decryption (both algorithms).
  3.1.Define a format for encrypted files.
  3.2.Implement encryption/decryption.
  3.3.Detection of errors.
  3.4.Add combo boxs for receiver selection.
  3.5.Keypair management.
4.Implement minilock compatibility.
  4.1.Key derivation from a password and email.
  4.2.Add a entropy calculator.
  4.3.Can decrypt/encrypt files from/to minilock.(optimistic)
  4.4.Can use the same email and password within ECCcryptor and minilock.
5.Implement PGP/GPG compatibility.
  5.1.Support RSA.
  
