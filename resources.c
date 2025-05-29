//자원을 가지고 있는지
int resourceHaved(Resource rs) {
    if (rs.wood==0 && rs.sand==0 && rs.coal==0 && rs.iron==0 && rs.gold==0)
        rs.have = 0;
    else
        rs.have = 1;
    return (rs.have);
}
