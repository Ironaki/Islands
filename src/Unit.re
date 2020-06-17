[%bs.raw {|require('./Unit.css')|}];

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
    | Land(terrain, startEnd, path)


// Nice Emoji 🏁🚴🚵⛰🌊
let unitDisplay = (u: unitType) => {
    switch (u) {
        | Water => React.string({js|🌊|js})
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

[@react.component]
let make = (~id, ~toggleUnit, ~unit as u, ~action) => {
    <button 
        id = id
        className=unitClass(u)
        onClick = {_ => toggleUnit(action)}
        >            
        {unitDisplay(u)}
    </button>
}