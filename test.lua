-- defines a factorial function
function fact(n)
    if n == 0 then
        return 1
    elseif n < 0 then
        print("<0")
    elseif condition then
        print("hello")
    else
        return n * fact(n - 1)
    end
end
print("enter a number:")
a = io.read("*number") -- read a number
print(fact(a))

while 5 > 2 do
    print("hello")
end


repeat
    print("hello2")
until 5 - 5 < 3


do 
    print("hello3")
end


  
--- Module for handling manifest files and tables.
-- Manifest files describe the contents of a LuaRocks tree or server.
-- They are loaded into manifest tables, which are then used for
-- performing searches, matching dependencies, etc.
local manif = {}

local core = require("luarocks.core.manif")
local persist = require("luarocks.persist")
local fetch = require("luarocks.fetch")
local dir = require("luarocks.dir")
local fs = require("luarocks.fs")
local cfg = require("luarocks.core.cfg")
local path = require("luarocks.path")
local util = require("luarocks.util")
local queries = require("luarocks.queries")
local type_manifest = require("luarocks.type.manifest")
