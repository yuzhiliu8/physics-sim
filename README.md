# Physics Simulator

### Collision Implementation (two moveable objects)
1. Traditional Conservation of Linear Momentum:
$$
m_1\vec{v_1} + m_2\vec{v_2} = m_1\vec{v_1'} + m_2 \vec{v_2'}
$$
- Restrict it to 1D, since collisions only act on normal axis: (using scalar projections)
$$
\hat{n} = normalize(pos_2 - pos_1) \\ {} \\
u_1 = comp_nv_1 = v_1 \cdot \hat{n} \\ {} \\
u_2 = comp_nv_2 = v_2 \cdot \hat{n} \\ {} \\ 
\underline{m_1u_1 + m_2u_2 = m_1u_1' + m_2u_2'}
$$

2. Coefficient of Resitution:
$$
e = -\frac{u_1' - u_2'}{u_1 - u_2} \\ {} \\
eu_1 - eu_2 = u_2' - u_1' \\ {} \\
\underline{u_2' = eu_1 - eu_2 + u_1'}
$$

3. Combine:
$$
m_1u_1 + m_2u_2 = m_1u_1' + m_2[eu_1 - eu_2 + u_1'] \\ {} \\
m_1u_1 + m_2u_2 - m_2eu_1 + m_2eu_2 = m_1u_1' + m_2u_1' \\ {} \\
\boxed{u_1' = \frac{m_1u_1 + m_2u_2 + m_2e(u_2 - u_1)}{m_1 + m_2}}
$$