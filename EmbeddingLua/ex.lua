a = 5*math.sin(45.3)
b = 343+34
c= 10/4
d  = "hello"

player = {Name = "Jassi",Family = "Wirral", Title= "Squire", Level = 20}

function CalledFromCPP1( a1,b1 )
print( "[CPP S3] CalledFromCPP1(" ..a1..", " ..b1.. ") called from C++ \n" )
return a1+b1
end

function CalledFromCPP2( a1,b1 )
print( "[CPP S4] CalledFromCPP2(" ..a1..", " ..b1.. ") called from C++ \n" )
c= HostFunction(a1+10,b1*4)

end

