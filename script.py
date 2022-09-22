def index_to_an(idx: int) -> str:
    return chr(idx % 8 - 1 + 96).upper() + int(idx / 8 + 1)
