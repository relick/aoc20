local socket = require"socket" -- for benchmarking

local function Solution(_tPrec)
	local oFile = io.open("input/18.txt", "r")
	local sLine = oFile:read()

	local nSum = 0

	while sLine ~= nil do
		local tOpStack = {}
		local tOutStack = {}
	
		local nOpTop = 0 -- avoid table removals
		local nOutTop = 0 -- do evaluation as we go
		for sToken in string.gmatch(sLine, "%S+") do
			if sToken == '+' or sToken == '*' then
				while nOpTop > 0 and tOpStack[nOpTop] ~= '(' and _tPrec[sToken] <= _tPrec[tOpStack[nOpTop]] do
					local nA = tOutStack[nOutTop]
					nOutTop = nOutTop - 1
					local nB = tOutStack[nOutTop]
					if tOpStack[nOpTop] == '+' then
						tOutStack[nOutTop] = nA + nB
					else -- item == '*'
						tOutStack[nOutTop] = nA * nB
					end
					nOpTop = nOpTop - 1
				end
				if nOpTop == #tOpStack then
					nOpTop = nOpTop + 1
					table.insert(tOpStack, sToken)
				else
					nOpTop = nOpTop + 1
					tOpStack[nOpTop] = sToken
				end
			else
				local sNum = string.match(sToken, "%d+")
				if sNum then
					if nOutTop == #tOutStack then
						nOutTop = nOutTop + 1
						table.insert(tOutStack, tonumber(sNum)) -- this is okay to do now and doesn't need to be after ( processing because they go on different stacks
					else
						nOutTop = nOutTop + 1
						tOutStack[nOutTop] = tonumber(sNum)
					end
				end
				for sOp in string.gmatch(sToken, "[%(%)]") do
					if sOp == '(' then
						if nOpTop == #tOpStack then
							nOpTop = nOpTop + 1
							table.insert(tOpStack, sOp)
						else
							nOpTop = nOpTop + 1
							tOpStack[nOpTop] = sOp
						end
					elseif sOp == ')' then
						while tOpStack[nOpTop] ~= '(' do
							local nA = tOutStack[nOutTop]
							nOutTop = nOutTop - 1
							local nB = tOutStack[nOutTop]
							if tOpStack[nOpTop] == '+' then
								tOutStack[nOutTop] = nA + nB
							else -- item == '*'
								tOutStack[nOutTop] = nA * nB
							end
							nOpTop = nOpTop - 1
						end
						nOpTop = nOpTop - 1 -- remove parenthesis pair
					end
				end
			end
		end
		while nOpTop > 0 do
			local nA = tOutStack[nOutTop]
			nOutTop = nOutTop - 1
			local nB = tOutStack[nOutTop]
			if tOpStack[nOpTop] == '+' then
				tOutStack[nOutTop] = nA + nB
			else -- item == '*'
				tOutStack[nOutTop] = nA * nB
			end
			nOpTop = nOpTop - 1
		end

		nSum = nSum + tOutStack[nOutTop]

		sLine = oFile:read()
	end
	
	return nSum
end

local nTotal = 0
local partA
local partB
for _=1, 1000 do
	local timeStart = socket.gettime()
	partA = Solution({ ['+'] = 1, ['*'] = 1, })
	partB = Solution({ ['+'] = 2, ['*'] = 1, })
	local timeEnd = socket.gettime()
	nTotal = nTotal + (timeEnd - timeStart)
end
local timeDiff = math.floor(nTotal * 1000) / 1000 -- 3dp ms
print("1000 runs: "..timeDiff.."ms\n")
print("Part A:\n"..partA.."\n\nPart B:\n"..partB)