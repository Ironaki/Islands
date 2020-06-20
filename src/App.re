[%bs.raw {|require('./App.css')|}];

[@react.component]
let make = () => {
  <main> <h3> {React.string("Islands - A* Path Search")} </h3> <World /> </main>;
};
