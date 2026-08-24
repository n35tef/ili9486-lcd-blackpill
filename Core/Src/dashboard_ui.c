#include "dashboard_ui.h"
#include "lvgl.h"
#include "lvgl_port.h"
#include "main.h"
#include <stdbool.h>

extern const lv_font_t hack_48;

#define COLOR_BG() lv_color_hex(0x070B16) /* near-black, deep Parisian-blue tint */
#define COLOR_NEON_CYAN() lv_color_hex(0x00E5FF)
#define COLOR_NEON_RED() lv_color_hex(0xFF2D55)
#define COLOR_NEON_YELLOW() lv_color_hex(0xFFD500)
#define TT_OPA_OFF LV_OPA_30
#define TT_OPA_ON LV_OPA_COVER
#define TT_CAPTION_OPA_OFF LV_OPA_10 /* captions stay near-invisible until their icon lights up */

/* RPM needle length is negative so it auto-scales to the gauge radius (radius - |len|). */
#define RPM_NEEDLE_LEN (-20)

static lv_obj_t* s_speed_label;
static lv_obj_t* s_rpm_scale;
static lv_obj_t* s_rpm_needle;

static lv_obj_t* s_batt_body;
static lv_obj_t* s_batt_nub;
static lv_obj_t* s_batt_cap;
static lv_obj_t* s_oil_body;
static lv_obj_t* s_oil_tip;
static lv_obj_t* s_oil_cap;
static lv_obj_t* s_pb_circle;
static lv_obj_t* s_pb_label;
static lv_obj_t* s_pb_arc_l;
static lv_obj_t* s_pb_arc_r;
static lv_obj_t* s_pb_cap;
static lv_obj_t* s_eng_tri;
static lv_obj_t* s_eng_label;
static lv_obj_t* s_eng_cap;
static lv_obj_t* s_telltale_row;

static void telltale_set_battery(bool on)
{
    lv_opa_t opa = on ? TT_OPA_ON : TT_OPA_OFF;
    lv_obj_set_style_border_color(s_batt_body, COLOR_NEON_RED(), 0);
    lv_obj_set_style_border_opa(s_batt_body, opa, 0);
    lv_obj_set_style_bg_opa(s_batt_body, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_color(s_batt_nub, COLOR_NEON_RED(), 0);
    lv_obj_set_style_border_opa(s_batt_nub, opa, 0);
    lv_obj_set_style_text_opa(s_batt_cap, on ? TT_OPA_ON : TT_CAPTION_OPA_OFF, 0);
}

static void telltale_set_oil(bool on)
{
    lv_opa_t opa = on ? TT_OPA_ON : TT_OPA_OFF;
    lv_obj_set_style_border_color(s_oil_body, COLOR_NEON_RED(), 0);
    lv_obj_set_style_border_opa(s_oil_body, opa, 0);
    lv_obj_set_style_line_color(s_oil_tip, COLOR_NEON_RED(), 0);
    lv_obj_set_style_line_opa(s_oil_tip, opa, 0);
    lv_obj_set_style_text_opa(s_oil_cap, on ? TT_OPA_ON : TT_CAPTION_OPA_OFF, 0);
}

static void telltale_set_parkbrake(bool on)
{
    lv_opa_t opa = on ? TT_OPA_ON : TT_OPA_OFF;
    lv_obj_set_style_border_color(s_pb_circle, COLOR_NEON_RED(), 0);
    lv_obj_set_style_border_opa(s_pb_circle, opa, 0);
    lv_obj_set_style_text_color(s_pb_label, COLOR_NEON_RED(), 0);
    lv_obj_set_style_text_opa(s_pb_label, opa, 0);
    lv_obj_set_style_arc_color(s_pb_arc_l, COLOR_NEON_RED(), LV_PART_INDICATOR);
    lv_obj_set_style_arc_opa(s_pb_arc_l, opa, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(s_pb_arc_r, COLOR_NEON_RED(), LV_PART_INDICATOR);
    lv_obj_set_style_arc_opa(s_pb_arc_r, opa, LV_PART_INDICATOR);
    lv_obj_set_style_text_opa(s_pb_cap, on ? TT_OPA_ON : TT_CAPTION_OPA_OFF, 0);
}

static void telltale_set_engine(bool on)
{
    lv_opa_t opa = on ? TT_OPA_ON : TT_OPA_OFF;
    lv_obj_set_style_line_color(s_eng_tri, COLOR_NEON_YELLOW(), 0);
    lv_obj_set_style_line_opa(s_eng_tri, opa, 0);
    lv_obj_set_style_text_color(s_eng_label, COLOR_NEON_YELLOW(), 0);
    lv_obj_set_style_text_opa(s_eng_label, opa, 0);
    lv_obj_set_style_text_opa(s_eng_cap, on ? TT_OPA_ON : TT_CAPTION_OPA_OFF, 0);
}

static void build_rpm_gauge(lv_obj_t* parent)
{
    s_rpm_scale = lv_scale_create(parent);
    lv_obj_set_size(s_rpm_scale, 200, 200);
    lv_obj_align(s_rpm_scale, LV_ALIGN_TOP_LEFT, 45, 40);
    lv_scale_set_mode(s_rpm_scale, LV_SCALE_MODE_ROUND_OUTER);
    lv_scale_set_range(s_rpm_scale, 0, 8000);
    lv_scale_set_total_tick_count(s_rpm_scale, 33);
    lv_scale_set_major_tick_every(s_rpm_scale, 4);
    lv_scale_set_label_show(s_rpm_scale, true);
    lv_scale_set_angle_range(s_rpm_scale, 270);
    lv_scale_set_rotation(s_rpm_scale, 135);

    /* major ticks are labelled 0-8 (x1000 rpm) instead of raw 0-8000 */
    static const char* rpm_labels[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", NULL};
    lv_scale_set_text_src(s_rpm_scale, rpm_labels);

    lv_obj_set_style_arc_color(s_rpm_scale, COLOR_NEON_CYAN(), LV_PART_MAIN);
    lv_obj_set_style_line_color(s_rpm_scale, COLOR_NEON_CYAN(), LV_PART_INDICATOR);
    lv_obj_set_style_line_width(s_rpm_scale, 3, LV_PART_INDICATOR);
    lv_obj_set_style_text_color(s_rpm_scale, COLOR_NEON_CYAN(), LV_PART_INDICATOR);
    lv_obj_set_style_text_font(s_rpm_scale, &lv_font_montserrat_16, LV_PART_INDICATOR);
    lv_obj_set_style_line_color(s_rpm_scale, COLOR_NEON_CYAN(), LV_PART_ITEMS);
    lv_obj_set_style_line_width(s_rpm_scale, 2, LV_PART_ITEMS);

    /* redline section */
    static lv_style_t style_redline;
    lv_style_init(&style_redline);
    lv_style_set_line_color(&style_redline, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_arc_color(&style_redline, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_text_color(&style_redline, lv_palette_main(LV_PALETTE_RED));
    lv_scale_section_t* redline = lv_scale_add_section(s_rpm_scale);
    lv_scale_set_section_range(s_rpm_scale, redline, 6500, 8000);
    lv_scale_set_section_style_main(s_rpm_scale, redline, &style_redline);
    lv_scale_set_section_style_indicator(s_rpm_scale, redline, &style_redline);
    lv_scale_set_section_style_items(s_rpm_scale, redline, &style_redline);

    s_rpm_needle = lv_line_create(s_rpm_scale);
    lv_obj_set_style_line_width(s_rpm_needle, 5, LV_PART_MAIN);
    lv_obj_set_style_line_rounded(s_rpm_needle, true, LV_PART_MAIN);
    lv_obj_set_style_line_color(s_rpm_needle, COLOR_NEON_CYAN(), LV_PART_MAIN);
    lv_scale_set_line_needle_value(s_rpm_scale, s_rpm_needle, RPM_NEEDLE_LEN, 0);

    lv_obj_t* rpm_unit_label = lv_label_create(s_rpm_scale);
    lv_label_set_text(rpm_unit_label, "x1000 RPM");
    lv_obj_set_style_text_color(rpm_unit_label, COLOR_NEON_CYAN(), 0);
    lv_obj_align(rpm_unit_label, LV_ALIGN_CENTER, 0, 45);
}

static void build_speed_label(lv_obj_t* parent)
{
    s_speed_label = lv_label_create(parent);
    lv_obj_set_style_text_font(s_speed_label, &hack_48, 0);
    lv_obj_set_style_text_color(s_speed_label, COLOR_NEON_CYAN(), 0);
    lv_label_set_text(s_speed_label, "0");
    /* fixed width + centered text so "km/h" below stays centered as digit count changes */
    lv_obj_set_width(s_speed_label, 160);
    lv_obj_set_style_text_align(s_speed_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(s_speed_label, LV_ALIGN_TOP_LEFT, 280, 90);

    lv_obj_t* unit_label = lv_label_create(parent);
    lv_label_set_text(unit_label, "km/h");
    lv_obj_set_style_text_color(unit_label, lv_color_hex(0x8A97A6), 0);
    lv_obj_align_to(unit_label, s_speed_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 4);
}

/* Fixed 50x50 area a telltale's shapes are built/centered in; caption sits below it in the flex
 * column, colored to match the icon's on-state hue and faded via *cap_out just like the icon. */
static lv_obj_t* create_telltale_slot(lv_obj_t* row, const char* caption, lv_color_t color,
                                      lv_obj_t** cap_out)
{
    lv_obj_t* col = lv_obj_create(row);
    lv_obj_remove_style_all(col);
    lv_obj_set_size(col, 90, 90);
    lv_obj_set_flex_flow(col, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(col, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_remove_flag(col, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t* icon_area = lv_obj_create(col);
    lv_obj_remove_style_all(icon_area);
    lv_obj_set_size(icon_area, 50, 50);

    lv_obj_t* cap = lv_label_create(col);
    lv_label_set_text(cap, caption);
    lv_obj_set_style_text_color(cap, color, 0);
    *cap_out = cap;

    return icon_area;
}

static void build_telltale_battery(lv_obj_t* icon_area)
{
    s_batt_body = lv_obj_create(icon_area);
    lv_obj_remove_style_all(s_batt_body);
    lv_obj_set_size(s_batt_body, 30, 20);
    lv_obj_set_style_radius(s_batt_body, 3, 0);
    lv_obj_set_style_border_width(s_batt_body, 2, 0);
    lv_obj_set_style_bg_opa(s_batt_body, LV_OPA_TRANSP, 0);
    lv_obj_center(s_batt_body);

    s_batt_nub = lv_obj_create(icon_area);
    lv_obj_remove_style_all(s_batt_nub);
    lv_obj_set_size(s_batt_nub, 8, 5);
    lv_obj_set_style_border_width(s_batt_nub, 2, 0);
    lv_obj_set_style_bg_opa(s_batt_nub, LV_OPA_TRANSP, 0);
    lv_obj_align_to(s_batt_nub, s_batt_body, LV_ALIGN_OUT_TOP_MID, 0, 2);

    telltale_set_battery(false);
}

static void build_telltale_oil(lv_obj_t* icon_area)
{
    s_oil_body = lv_obj_create(icon_area);
    lv_obj_remove_style_all(s_oil_body);
    lv_obj_set_size(s_oil_body, 24, 24);
    lv_obj_set_style_radius(s_oil_body, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_border_width(s_oil_body, 2, 0);
    lv_obj_set_style_bg_opa(s_oil_body, LV_OPA_TRANSP, 0);
    lv_obj_align(s_oil_body, LV_ALIGN_BOTTOM_MID, 0, -4);

    /* teardrop tip: closed triangle sitting on top of the circle body */
    static lv_point_precise_t oil_tip_pts[] = {{0, 10}, {14, 10}, {7, 0}, {0, 10}};
    s_oil_tip = lv_line_create(icon_area);
    lv_obj_set_size(s_oil_tip, 14, 10);
    lv_line_set_points(s_oil_tip, oil_tip_pts, 4);
    lv_obj_set_style_line_width(s_oil_tip, 2, 0);
    lv_obj_set_style_line_rounded(s_oil_tip, true, 0);
    lv_obj_align_to(s_oil_tip, s_oil_body, LV_ALIGN_OUT_TOP_MID, 0, 6);

    telltale_set_oil(false);
}

static void build_telltale_parkbrake(lv_obj_t* icon_area)
{
    s_pb_circle = lv_obj_create(icon_area);
    lv_obj_remove_style_all(s_pb_circle);
    lv_obj_set_size(s_pb_circle, 30, 30);
    lv_obj_set_style_radius(s_pb_circle, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_border_width(s_pb_circle, 2, 0);
    lv_obj_set_style_bg_opa(s_pb_circle, LV_OPA_TRANSP, 0);
    lv_obj_center(s_pb_circle);

    s_pb_label = lv_label_create(icon_area);
    lv_label_set_text(s_pb_label, "P");
    lv_obj_center(s_pb_label);

    /* two partial arcs flanking the circle, mimicking the "(P)" parking-brake symbol */
    s_pb_arc_l = lv_arc_create(icon_area);
    lv_obj_set_size(s_pb_arc_l, 42, 42);
    lv_arc_set_bg_angles(s_pb_arc_l, 100, 260);
    lv_arc_set_value(s_pb_arc_l, 100);
    lv_obj_remove_style(s_pb_arc_l, NULL, LV_PART_KNOB | LV_STATE_ANY);
    lv_obj_set_style_arc_opa(s_pb_arc_l, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_arc_width(s_pb_arc_l, 3, LV_PART_INDICATOR);
    lv_obj_remove_flag(s_pb_arc_l, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_center(s_pb_arc_l);

    s_pb_arc_r = lv_arc_create(icon_area);
    lv_obj_set_size(s_pb_arc_r, 42, 42);
    lv_arc_set_bg_angles(s_pb_arc_r, 280, 80);
    lv_arc_set_value(s_pb_arc_r, 100);
    lv_obj_remove_style(s_pb_arc_r, NULL, LV_PART_KNOB | LV_STATE_ANY);
    lv_obj_set_style_arc_opa(s_pb_arc_r, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_arc_width(s_pb_arc_r, 3, LV_PART_INDICATOR);
    lv_obj_remove_flag(s_pb_arc_r, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_center(s_pb_arc_r);

    telltale_set_parkbrake(false);
}

static void build_telltale_engine(lv_obj_t* icon_area)
{
    static lv_point_precise_t eng_tri_pts[] = {{17, 2}, {2, 32}, {32, 32}, {17, 2}};
    s_eng_tri = lv_line_create(icon_area);
    lv_obj_set_size(s_eng_tri, 34, 34);
    lv_line_set_points(s_eng_tri, eng_tri_pts, 4);
    lv_obj_set_style_line_width(s_eng_tri, 2, 0);
    lv_obj_set_style_line_rounded(s_eng_tri, true, 0);
    lv_obj_center(s_eng_tri);

    s_eng_label = lv_label_create(icon_area);
    lv_label_set_text(s_eng_label, "!");
    lv_obj_align(s_eng_label, LV_ALIGN_CENTER, 0, 6);

    telltale_set_engine(false);
}

static void build_telltale_row(lv_obj_t* parent)
{
    lv_obj_t* row = lv_obj_create(parent);
    lv_obj_remove_style_all(row);
    lv_obj_set_size(row, 460, 90);
    lv_obj_align(row, LV_ALIGN_BOTTOM_MID, 0, -8);
    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER);
    lv_obj_remove_flag(row, LV_OBJ_FLAG_SCROLLABLE);

    build_telltale_battery(create_telltale_slot(row, "BATT", COLOR_NEON_RED(), &s_batt_cap));
    build_telltale_oil(create_telltale_slot(row, "OIL", COLOR_NEON_RED(), &s_oil_cap));
    build_telltale_parkbrake(create_telltale_slot(row, "BRAKE", COLOR_NEON_RED(), &s_pb_cap));
    build_telltale_engine(create_telltale_slot(row, "ENGINE", COLOR_NEON_YELLOW(), &s_eng_cap));
    s_telltale_row = row;
}

static void telltale_set_all(bool on)
{
    telltale_set_battery(on);
    telltale_set_oil(on);
    telltale_set_parkbrake(on);
    telltale_set_engine(on);
}

/* idx: 0=battery, 1=oil, 2=brake, 3=engine; any other value lights none */
static void telltale_set_only(int32_t idx)
{
    telltale_set_battery(idx == 0);
    telltale_set_oil(idx == 1);
    telltale_set_parkbrake(idx == 2);
    telltale_set_engine(idx == 3);
}

static void speed_tick_cb(lv_timer_t* t)
{
    (void)t;
    /* Advance exactly one km/h per displayed frame: the frame counter only moves when a frame
     * has actually been flushed to the panel, so the sweep runs at the true max frame rate while
     * never skipping a value. Runs every main-loop pass; the guard makes it a cheap no-op between
     * frames. */
    static uint32_t last_frame;
    uint32_t frame = lvgl_port_get_frame_count();
    if (frame == last_frame)
        return;
    last_frame = frame;

    /* triangle-wave sweep 0 -> 180 -> 0 km/h */
    static int32_t speed = 0;
    static int32_t speed_dir = 1;
    speed += speed_dir;
    if (speed >= 180)
    {
        speed = 180;
        speed_dir = -1;
    }
    else if (speed <= 0)
    {
        speed = 0;
        speed_dir = 1;
    }
    int32_t rpm = 800 + speed * 40;

    lv_label_set_text_fmt(s_speed_label, "%d", (int)speed);
    lv_scale_set_line_needle_value(s_rpm_scale, s_rpm_needle, RPM_NEEDLE_LEN, rpm);
}

static void tick_cb(lv_timer_t* t)
{
    (void)t;
    static uint32_t phase_ms = 0;
    phase_ms += 50;

    /* telltale demo pattern: 1-2-3-4, off-on-off-on-off, 4-3-2-1, off-on-off-on-off, repeat */
    static const int32_t tt_steps[] = {
        0, 1, 2, 3, -1, -2, -1, -2, -1, 3, 2, 1, 0, -1, -2, -1, -2, -1,
    };
    const uint32_t step_ms = 100;
    uint32_t step_idx = (phase_ms / step_ms) % (sizeof(tt_steps) / sizeof(tt_steps[0]));
    int32_t step = tt_steps[step_idx];
    if (step == -1)
        telltale_set_all(false);
    else if (step == -2)
        telltale_set_all(true);
    else
        telltale_set_only(step);

    /* merge all icon/caption changes into one invalidate so they flush to the panel together */
    lv_obj_invalidate(s_telltale_row);
}

void dashboard_ui_init(void)
{
    lv_obj_t* scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, COLOR_BG(), 0);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);

    build_rpm_gauge(scr);
    build_speed_label(scr);
    build_telltale_row(scr);

    /* period 0: lv_tick has 1ms resolution (SysTick), so this fires every main-loop pass -
     * as fast as lv_timer_handler() itself is called, which is the real achievable ceiling */
    lv_timer_create(speed_tick_cb, 0, NULL);
    lv_timer_create(tick_cb, 50, NULL);
}
