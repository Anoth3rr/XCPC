ld angle(ld a, ld b, ld c) { // 余弦定理
    ld val = acos((a * a + b * b - c * c) / (2.0 * a * b)); // 计算弧度
    return val;
}
