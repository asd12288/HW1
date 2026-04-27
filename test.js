captureMouse = () => {
  document.addEventListener("mousemove", (event) => {
    console.log("X Position", event.clientX);
    console.log("Y Position", event.clientY);
  });
};
