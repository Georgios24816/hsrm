def Summe(n):
	if (n <= 0):
		return 0;
	else:
		return n + Summe(n - 1);

print(Summe(5));
