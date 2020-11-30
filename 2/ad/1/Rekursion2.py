def foo(a, b):
    if a == 0:
        print(0)
        return 0;

    elif a != 0:
        s = "r(b, a, b)"
        s = s.replace("b", str(b))
        s = s.replace("a", str(a))
        print(s)
        return bar(b, a, b)

def bar(i, a, b):
    if i != 0:
        s = "a + r(i - 1, a, b)"
        s = s.replace("a", str(a))
        s = s.replace("b", str(b))
        s = s.replace("i", str(i))
        print(s)
        return a + bar(i - 1, a, b)
    elif i == 0:
        s = "foo(a - 1, b)"
        s = s.replace("a", str(a))
        s = s.replace("b", str(b))
        print(s)
        return foo(a - 1, b)

print(foo(4, 2))
