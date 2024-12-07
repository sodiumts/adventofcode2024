files = ['input.txt'] 
with open(files[0], 'r') as f:
    data = [x.strip().split(": ") for x in f.readlines()]

equations = [[int(i), list(map(int, j.split(" ")))] for i,j in data]

def calc(cur, vals, evals):
    if len(vals) == 0: return [cur]
    return sum([calc(e(cur,vals[0]), vals[1:], evals) for e in evals], [])

def run(equations, partTwo=False):
    total = 0
    evals = [lambda a,b: a+b, lambda a,b: a*b]
    if partTwo: evals.append(lambda a,b: int(str(a)+str(b)))
    for ans, vals in equations:
        pos = calc(vals[0], vals[1:], evals)
        if ans in pos:
            total += ans
    return total
    
print("Part 2: ", run(equations, True))
