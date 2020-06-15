[%bs.raw {|require('./Unit.css')|}];

type unitType =
    | Road
    | Mountain
    | Water;

let unitDisplay = (u: unitType) => {
    switch (u) {
        | Road => React.string({js| |js})
        | Mountain => React.string({js|🗻|js})
        | Water => React.string({js|🌊|js})
    };
};

let unitClass = (u: unitType) => {
    switch (u) {
        | Road => "Road"
        | Mountain => "Mountain"
        | Water => "Water"
    }
}

let unitChange = (u: unitType) => {
    switch (u) {
        | Road => Mountain
        | Mountain => Water
        | Water => Road
    };
};

let unitCost = (u: unitType) => {
    switch (u) {
    | Road => 1
    | Mountain => 3
    | Water => -1  // Should never call on Water
    };
};

type state = {
    unit: unitType
};

let initialState = {
    unit: Road
};

/* let reducer = (state, _) => {
    {unit: unitChange(state.unit)}
}; */

[@react.component]
let make = (~id, ~toggleUnit, ~unit as u, ~coord) => {
    <button 
        id = id
        className=unitClass(u)
        onClick = {_ => toggleUnit(coord)}
        >            
        {unitDisplay(u)}
    </button>
}