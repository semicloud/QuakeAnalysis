def get_marker_color(mag: float):
    if mag < 4.0:
        return 'go'
    elif 4.0 <= mag < 6:
        return 'mo'
    elif mag >= 6.0:
        return 'ro'
