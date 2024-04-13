#include <furi.h>
#include <stdio.h>
#include <gui/gui.h>
#include <input/input.h>
#include <notification/notification_messages.h>

/* generated by fbt from .png files in images folder */
#include <hello_flipper_zero_icons.h>

static void draw_callback(Canvas* canvas, void* ctx) {
    UNUSED(ctx);

    canvas_clear(canvas);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 0, 30, "Hello Flipper Zero!");
}

static void input_callback(InputEvent* input_event, void* ctx) {
    // Checking the context is not null
    furi_assert(ctx);
    FuriMessageQueue* event_queue = ctx;

    furi_message_queue_put(event_queue, input_event, FuriWaitForever);
}

int32_t hello_flipper_zero_app(void* p) {
    UNUSED(p);
    // FURI_LOG_I("TEST", "Hello world");
    // FURI_LOG_I("TEST", "I'm at_beat_zero!");
    // Current event of InputEvent type
    InputEvent event;
    // Event queue for 8 elements of InputEvent size
    FuriMessageQueue* event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));

    // Create a new view port
    ViewPort* view_port = view_port_alloc();
    // Create a rendering callback, without context
    view_port_draw_callback_set(view_port, draw_callback, NULL);
    // Create a callback of keystrokes, pass as a context
    // Our message queue to feed these events into.
    view_port_input_callback_set(view_port, input_callback, event_queue);

    // Create application GUI
    Gui* gui = furi_record_open(RECORD_GUI);
    // Connect view port to GUI in full screen mode
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    // Infinite loop of event queue processing
    while(1) {
        // Select an event from the queue into the event variable (wait infinitely long if the queue is empty)
        // and verify that we're able to do it.
        furi_check(furi_message_queue_get(event_queue, &event, FuriWaitForever) == FuriStatusOk);

        // If the "back" button is pressed, we exit the loop and therefore the application
        if(event.key == InputKeyBack) {
            break;
        }
    }

    // Special clearing of memory occupied by the queue
    furi_message_queue_free(event_queue);

    // Clean the created objects associated with the interface
    gui_remove_view_port(gui, view_port);
    view_port_free(view_port);
    furi_record_close(RECORD_GUI);

    return 0;
}
