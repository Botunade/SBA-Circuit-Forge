import flet as ft

def main(page: ft.Page):
    page.title = "Circuit Simulation"
    page.vertical_alignment = ft.MainAxisAlignment.START
    page.horizontal_alignment = ft.CrossAxisAlignment.CENTER

    # Controls for adding components and running simulation - optimized for touch
    control_buttons = ft.Row(
        [
            ft.ElevatedButton(
                "Add Resistor",
                icon=ft.icons.TUNE, # Example icon
                on_click=lambda e: print("Add Resistor clicked"),
                height=50, # Larger button for touch
                width=150,
            ),
            ft.ElevatedButton(
                "Add Capacitor",
                icon=ft.icons.CAPACITOR, # Placeholder icon
                on_click=lambda e: print("Add Capacitor clicked"),
                height=50,
                width=150,
            ),
            ft.ElevatedButton(
                "Run Simulation",
                icon=ft.icons.PLAY_ARROW,
                on_click=lambda e: print("Run Simulation clicked"),
                height=50,
                width=180, # Slightly wider for "Run Simulation"
                style=ft.ButtonStyle(bgcolor={ft.MaterialState.DEFAULT: ft.colors.GREEN_700}),
            ),
        ],
        alignment=ft.MainAxisAlignment.SPACE_EVENLY, # Distribute buttons evenly
        wrap=True, # Allow buttons to wrap if screen is too narrow
    )

    # Circuit Canvas - ready for C++ integration
    circuit_canvas = ft.Container(
        content=ft.Canvas(
            [
                # Placeholder for drawing elements - this area will be controlled by C++ physics engine
                ft.Text(
                    "Circuit Canvas: C++ Physics Engine Ready",
                    size=18,
                    offset=ft.Offset(0.5, 0.5), # Centered text
                    weight=ft.FontWeight.BOLD,
                    color=ft.colors.GREY_600,
                ),
            ],
            expand=True,
            background_paint=ft.Paint(
                style=ft.PaintingStyle.FILL,
                stroke_width=1,
                color=ft.colors.WHITE,
            ),
        ),
        expand=True,
        bgcolor=ft.colors.BLUE_GREY_50, # A light background for the container
        alignment=ft.alignment.center,
        border_radius=ft.border_radius.all(10),
        padding=10,
        margin=ft.margin.only(top=10, bottom=10), # Add some vertical margin
    )

    page.add(
        ft.Column(
            [
                control_buttons,
                ft.Divider(),
                circuit_canvas,
            ],
            alignment=ft.MainAxisAlignment.START,
            horizontal_alignment=ft.CrossAxisAlignment.CENTER,
            expand=True,
        )
    )

if __name__ == "__main__":
    ft.app(target=main)
