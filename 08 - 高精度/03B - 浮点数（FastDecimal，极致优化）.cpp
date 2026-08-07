/*
用途：03A Decimal 的卡常替代；精度改为编译期常量，避免每次运算读取/维护动态 scale。
依赖：先粘贴 01B，再粘贴 02B。FastDecimal<P> 与 FastFixedPoint<P> 语义相同，P 为小数位数。
取舍：保留常用四则、比较、pow、输入输出；需要 sqrt、动态 setPre、round 时使用 03A。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <int P>
using Decimal = FixedPoint<P>;
