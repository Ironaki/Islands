type terrain =
    | Road
    | Mountain; 

type path =
    | InPath
    | NotPath;

type startEnd =
    | Ordinary
    | Start
    | End;

type unitType =
    | Water
    | Land(terrain, startEnd, path);


// Nice Emoji 🏁🚴🚵⛰🌊
let unitDisplay = (u: unitType) => {
    switch (u) {
        | Water => React.string({js| |js})
        | Land(_, End, _) => React.string({js|🏁|js})
        | Land(Road, Start, _) => React.string({js|🚴|js})
        | Land(Mountain, Start, _) => React.string({js|🚵|js})
        | Land(Road, Ordinary, _) => React.string({js| |js})
        | Land(Mountain, Ordinary, _) => React.string({js|⛰|js})
    };
};


// For CSS use
let unitClass = (u: unitType) => {
    switch (u) {
        | Water => "Water"
        | Land(_, _, InPath) => "InPath"
        | Land(Road, _, NotPath) => "Road"
        | Land(Mountain, _, NotPath) => "Mountain"
    }
}

let setPoint = (u: unitType, pointType) => {
    switch (u) {
        | Land(Road, _, _) => Land(Road, pointType, NotPath)
        | Land(Mountain, _, _) => Land(Mountain, pointType, NotPath)
        | Water => Water
    }
}

let setStart = (u: unitType) => setPoint(u, Start)

let setEnd = (u: unitType) => setPoint(u, End)

let setOrdinary = (u: unitType) => setPoint(u, Ordinary)

let unitChange = (u: unitType) => {
    switch (u) {
        | Land(Road, Ordinary, NotPath) => Land(Mountain, Ordinary, NotPath)
        | Land(Mountain, Ordinary, NotPath) => Water
        | Water => Land(Road, Ordinary, NotPath)
        | _ => Land(Road, Ordinary, NotPath)
    };
};

let unitCost = (u: unitType) => {
    switch (u) {
    | Land(Road, _, _) => 1
    | Land(Mountain, _, _) => 3
    | Water => max_int
    };
};

let unitToPath = (u: unitType) => {
    switch (u) {
        | Land(Road, Ordinary, NotPath) => Land(Road, Ordinary, InPath)
        | Land(Road, Start, NotPath) => Land(Road, Start, InPath)
        | Land(Road, End, NotPath) => Land(Road, End, InPath)
        | Land(Mountain, Ordinary, NotPath) => Land(Mountain, Ordinary, InPath)
        | Land(Mountain, Start, NotPath) => Land(Mountain, Start, InPath)
        | Land(Mountain, End, NotPath) => Land(Mountain, Start, InPath)
        | Land(Road, Ordinary, InPath) => Land(Road, Ordinary, NotPath)
        | Land(Road, Start, InPath) => Land(Road, Start, NotPath)
        | Land(Road, End, InPath) => Land(Road, End, NotPath)
        | Land(Mountain, Ordinary, InPath) => Land(Mountain, Ordinary, NotPath)
        | Land(Mountain, Start, InPath) => Land(Mountain, Start, NotPath)
        | Land(Mountain, End, InPath) => Land(Mountain, End, NotPath)
        | Water => Water  // Not valid input
    };
};

let isWater = (u: unitType) => {
    switch (u) {
        | Water => true
        | Land(_, _, _) => false
    };
};

let isStartEnd = (u: unitType) => {
    switch (u) {
        | Land(_, Start, _) => true
        | Land(_, End, _) => true
        | _ => false
    }
};

let randomUnitInit = () => {
    //Random.init(int_of_float(Js.Date.now()));
    let r = Random.int(10);
    if (r < 2) {
        Land(Mountain, Ordinary, NotPath)
    } else if (r >= 4) {
        Land(Road, Ordinary, NotPath)
    } else {
        Water
    }
};

let randomStartEnd = ((), pointType) => {
    //Random.init(int_of_float(Js.Date.now()));
    let r = Random.int(5);
    Js.log(r);
    if (r < 2) {
        Land(Mountain, pointType, NotPath)
    } else {
        Land(Road, pointType, NotPath)
    }
};

let randomStart = () => randomStartEnd((), Start);

let randomEnd = () => randomStartEnd((), End);


[@react.component]
let make = (~id, ~dispatch, ~unit as u, ~action, ~foundPath, ~startEndNotSet) => {
    <button 
        id = id
        className=(unitClass(u))
        disabled=(switch (foundPath, startEndNotSet, isWater(u), isStartEnd(u)) {
                    | (true, _, _, _) => true
                    | (false, false, _, _) => false
                    | (false, true, true, _) => true
                    | (false, true, false, false) => false
                    | (false, true, false, true) => true
                    })
        onClick=(_ => dispatch(action))
        >            
        {unitDisplay(u)}
    </button>
}