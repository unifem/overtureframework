
#beginMacro MAC(x)
Here is macro two
x
#endMacro

#beginMacro ARG(x)
***Here is macro one x *****
#endMacro

c

MAC($ARG(hi))

c

