def tree_insert(t, x):
    match t:
        case (): return ((), x, (), 0)
        case (l, y, r, h): return BT(tree_insert(l, x), y, r) if x < y else BT(l, y, tree_insert(r, x))


def H(x):
    match x:
        case (): return 0
        case (l, y, r, h): return h

def BT(l, x, r):
    lh, rh = H(l), H(r)
    if lh == rh +2:
        ll, lx, rl, hl = l
        if H(ll) > H(rl):
            return BT(ll, lx, BT(rl, x, r)) 
        else:
            lrl, rlx, rrl, hrl = rl
            return BT(BT(ll, lx, lrl), rlx, BT(rrl, x, r))
    elif rh == lh+2:
        lr, rx, rr, hr = r
        if H(rr) > H(lr):
            return BT(BT(l, x, lr), rx, rr)
        else:
            llr, lrx, rlr, hlr = lr
            return BT(BT(l, x, llr,), lrx , BT(rlr, rx, rr))
    else:
        return (l, x, r, (1+max(H(l), H(r))))
tree = ()

for i in range(1000000):
    tree =tree_insert(tree, i)

print(H(tree))

