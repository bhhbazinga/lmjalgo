--[[
FileName: test.lua
Module: 
Author: turbobhh
Mail: turbobhh@gmail.com
CreatedTime: Sun 11 Jun 2017 06:32:45 PM CST
]]

local function dump(value, description, nesting)
	if type(nesting) ~= "number" then
		nesting = 10
	end

	local lookupTable = {}
	local result =  {}

	local function _v(v)
		if type(v) == "string" then
			v = "\"" .. v .. "\""
		end
		return tostring(v)
	end

	local function _dump(value, description, indent, nest, keylen)
		description = description or "<var>"
		local spc = ""
		if type(keylen) == "number" then
			spc = string.rep(" ",keylen - string.len(_v(description)))
		end

		if type(value) ~= "table" then
			result[#result + 1] = string.format("%s%s%s = %s", indent, _v(description), spc, _v(value))
		elseif lookupTable[value] then
			result[#result + 1] = string.format("%s%s%s = *REF*", indent, description, spc)
		else
			lookupTable[value] = true
			if nest > nesting then
				result[#result + 1] = string.format("%s%s = *MAX NESTING*", indent, description)
			else
				result[#result + 1] = string.format("%s%s = {" , indent, _v(description))
				local indent2 = indent .. "    "
				local keys = {}
				local keylen = 0
				local values = {}
				for k, v in pairs(value) do
					keys[#keys + 1] = k
					local vk = _v(k)
					local vk1 = string.len(vk)
					if vk1 > keylen then
						keylen = vk1
					end
					values[k] = v
				end
				table.sort(keys,function(a, b)
					if type(a) == "number" and type(b) == "number" then
						return a < b
					else
						return tostring(a) < tostring(b)
					end
				end)

				for i, k in pairs(keys) do
					_dump(values[k], k, indent2,nest + 1,keylen)
				end
				result[#result + 1] = string.format("%s}", indent)
			end
		end
	end
	_dump(value,description, "- ", 1)

	table.insert(result, 1, "\n")
	local str = table.concat(result, "\n")
	print(str)
	return str
end

local lmjalgo = require "lmjalgo"

local pais = {
	{0, 0, 1, 0, 2, 0, 1, 0, 0, 4},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 1, 0, 2, 1, 1, 0, 0, 5},
	{0, 0, 0, 0, 0, 2, 0, 0, 0, 2},
}

local lnums = {}
for i = 11, 47 do
	table.insert(lnums, i)
end

local t1 = os.clock()
local data = lmjalgo.g_gettingdata(pais, {17}, lnums, {19}, ALL)
local mask = lmjalgo.g_gethumask(pais, {17}, {19}, ALL)

local t2 = os.clock()
print(t2 - t1)
print(mask)
dump(data, "ting_data")
dump(lmjalgo)
