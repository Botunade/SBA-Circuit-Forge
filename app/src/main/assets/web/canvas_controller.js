document.addEventListener('DOMContentLoaded', () => {
    const canvas = document.getElementById('circuitCanvas');
    const ctx = canvas.getContext('2d');
    const circuitDataPre = document.getElementById('circuitData');

    // Function to draw a component (placeholder)
    function drawComponent(ctx, component, x, y) {
        ctx.strokeStyle = 'black';
        ctx.lineWidth = 2;
        if (component.type === 'resistor') {
            ctx.strokeRect(x, y, 50, 20); // Example drawing for a resistor
            ctx.fillText(component.value, x + 5, y + 15);
        } else if (component.type === 'capacitor') {
            ctx.beginPath();
            ctx.moveTo(x, y);
            ctx.lineTo(x + 20, y);
            ctx.moveTo(x + 20, y - 10);
            ctx.lineTo(x + 20, y + 10);
            ctx.moveTo(x + 30, y - 10);
            ctx.lineTo(x + 30, y + 10);
            ctx.moveTo(x + 30, y);
            ctx.lineTo(x + 50, y);
            ctx.stroke();
            ctx.fillText(component.value, x + 15, y + 25);
        }
        // Add more component types here
    }

    // Function to fetch and render circuit state
    async function fetchAndRenderCircuitState() {
        try {
            const response = await fetch('http://127.0.0.1:18080/get_circuit_state');
            const data = await response.json();

            circuitDataPre.textContent = JSON.stringify(data, null, 2);

            if (data.status === 'success' && data.data && data.data.components) {
                ctx.clearRect(0, 0, canvas.width, canvas.height); // Clear canvas
                let xOffset = 50;
                let yOffset = 50;
                data.data.components.forEach(component => {
                    drawComponent(ctx, component, xOffset, yOffset);
                    xOffset += 100; // Move for next component
                });
            }

        } catch (error) {
            console.error('Error fetching circuit state:', error);
            circuitDataPre.textContent = `Error: ${error.message}. Make sure the C++ server is running on http://127.0.0.1:18080`;
        }
    }

    // Fetch and render initially
    fetchAndRenderCircuitState();

    // Optionally, refresh every few seconds
    // setInterval(fetchAndRenderCircuitState, 5000);
});
