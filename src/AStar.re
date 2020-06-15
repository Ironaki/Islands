type node = {
    heuristic: int,
    steps: int,
    row: int,
    col: int
};

let minQueue = PriorityQueue.make((nodeA, nodeB) => nodeA.heuristic-nodeB.heuristic);



PriorityQueue.push(minQueue, {
    heuristic: 0,
    steps: 0,
    row: 0,
    col: 0
});


Js.log(minQueue->PriorityQueue.pop);