//*
// Return the dimension the string would have if printed in
// the specified size and font.
Vec2 messure_text(const char *string, f32 size, AssetID font_id);

//*
// Render the "string" to the screen. Where the line-left corner
// of the text is located at (x, y). "border" adds a black border
// around the text and "edge" is the thickness of the bezels.
void draw_text(const char *string, f32 x, f32 y, f32 size, AssetID font_id,
               Vec4 color = V4(1, 1, 1, 1), f32 edge = 0.2,
               bool border = false);

