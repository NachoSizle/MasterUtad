import math
import random
import matplotlib.pyplot as plt


class Point:
    def __init__(self, x_init, y_init):
        self.x = x_init
        self.y = y_init
        self.angle = 0

    def __repr__(self):
        return "".join(["[", str(self.x), ", ", str(self.y), "]"])


class Vector:

    def __init__(self, point_a, point_b):
        self.point_a = point_a
        self.point_b = point_b
        self.vec = [point_b.x - point_a.x, point_b.y - point_a.y]

    def change_vector(self):
        return [-self.vec[1], self.vec[0]]

    def vector_module(self):
        return math.sqrt((self.vec[0])**2+(self.vec[1])**2)

    def get_angle(self):
        return math.atan2(self.point_b.y - self.point_a.y, self.point_b.x - self.point_a.x)

    def __repr__(self):
        return "".join(["Vector(", str(self.vec[0]), ", ", str(self.vec[1]), ")"])


class Polygon:
    def __init__(self, point_list):
        self.point_list = point_list
        self.init_point = Point(0, 0)

    def get_sides(self):
        print(self.point_list)
        sides = []
        num_points = len(self.point_list)
        for index in range(0, num_points):
            next_pos = index + 1
            if (next_pos < num_points):
                point_a = self.point_list[index]
                point_b = self.point_list[next_pos]
                vector = Vector(point_a, point_b)
                sides.append(vector)
        print("Sides: {}".format(sides))
        return sides

    def getConvexHull(self):
        num_size = len(self.point_list)
        if num_size < 3:
            print("ConvexHull not possible \n")
            return
        polRes = Polygon([])
        min_x = 0
        max_x = 0

        for point_index in range(num_size):
            if self.point_list[point_index].x < self.point_list[min_x].x:
                min_x = point_index
            if self.point_list[point_index].x > self.point_list[max_x].x:
                max_x = point_index

        getQuickHull(self.point_list, num_size,
                     self.point_list[min_x], self.point_list[max_x], 1, polRes.point_list)
        getQuickHull(self.point_list, num_size,
                     self.point_list[min_x], self.point_list[max_x], -1, polRes.point_list)

        polRes.setInitPoint()
        polRes.orderPoints()

        return polRes

    def orderPoints(self):
        for point in self.point_list:
            vector = Vector(self.init_point, point)
            point.angle = vector.get_angle()

        self.point_list = sorted(self.point_list, key=getAngle)

    def setInitPoint(self):
        min_y = 10000
        max_x = -10000
        minor = Point(max_x, min_y)

        for point in self.point_list:
            if point.y < min_y:
                minor = point
                min_y = minor.y
                if point.x > minor.x:
                    max_x = minor.x
        self.init_point = minor

    def drawPolygon(self):
        x_coor = []
        y_coor = []

        self.point_list.append(self.point_list[0])

        for point in self.point_list:
            x_coor.append(point.x)
            y_coor.append(point.y)

        plt.plot(x_coor, y_coor, "ro-", linewidth=1.5, markersize=6)

    def __repr__(self):
        return "".join(["Polygon(", str(self.point_list), ")"])


def getAngle(point):
    return point.angle


def getQuickHull(pol, num, pointA, pointB, side, polRes):
    ind = -1
    max_dist = 0

    for point in range(num):
        temp = lineDist(pointA, pointB, pol[point])
        if area2(pointA, pointB, pol[point]) == side:
            if temp > max_dist:
                ind = point
                max_dist = temp

    if ind == -1:
        if notExistPoint(polRes, pointA):
            polRes.append(pointA)
        if notExistPoint(polRes, pointB):
            polRes.append(pointB)
        return

    getQuickHull(pol, num, pol[ind], pointA, -
                 area2(pol[ind], pointA, pointB), polRes)
    getQuickHull(pol, num, pol[ind], pointB, -
                 area2(pol[ind], pointB, pointA), polRes)


def notExistPoint(point_list, point):
    found = True
    for p in point_list:
        if p.x == point.x and p.y == point.y:
            found = False
            return
    return found


def aux(point_a, point_b, point_c):
    return (point_c.y - point_a.y) * (point_b.x - point_a.x) - (point_b.y - point_a.y) * (point_c.x - point_a.x)


def lineDist(point_a, point_b, point_c):
    return abs(aux(point_a, point_b, point_c))


def area2(point_a, point_b, point_c):
    val = aux(point_a, point_b, point_c)
    if val > 0:
        return 1
    if val < 0:
        return -1
    return 0


def prod_escalar(v1, v2):
    return ((v1[0]*v2[0])+(v1[1]*v2[1]))


def calcCollision(polygon_list):
    # Eje de proyeccion
        # perpendicular (-y,x)
    # Sacar las proyecciones a ese eje
        # Proyectar todos los lados del poligono que he sacado el plano de proyeccion
            # Cada proyeccion es ((prod_escalar(lado a probar, plano de proyeccion)) / (mod(plano de proyeccion)^2)) * vector(plano de proyeccion)
        # Una vez proyectados todos los lados, cogemos la coordenada mas pequeña de las pequeñas y la mas grande de las grandes -->
        # toda la proyeccion del polígono en el plano

        # Repetir los 3 pasos anteriores con el otro polígono en el mismo eje de proyeccion
        # Comprobar si los extremos del primer poligono pertenecen al conjunto de puntos del segundo poligono
    res = True
    polygon = polygon_list[0]
    first_sides = polygon.get_sides()
    polygon = polygon_list[1]
    second_sides = polygon.get_sides()

    init_side = 0
    origin_point = Point(0, 0)
    plane_proyection = Vector(Point(0, 0), Point(0, 0))
    perpen_plane = plane_proyection

    for side in first_sides:
        if res:
            init_side = side

            plane_proyection.vec = init_side.change_vector()

            perpen_plane.vec = [
                plane_proyection.vec[1], plane_proyection.vec[0]]
            mod_perpen_plane = math.sqrt(
                (perpen_plane.vec[0])**2+(perpen_plane.vec[1])**2)

            proy_origin_first_polygon = []
            proy_origin_second_polygon = []
            proyections_first_polygon = []
            proyections_second_polygon = []

            for side_first in first_sides:
                point = side_first.point_a
                vec = Vector(origin_point, point)
                proy = (prod_escalar(vec.vec, perpen_plane.vec) /
                        mod_perpen_plane)
                proy_origin_first_polygon.append(proy)

            proyections_first_polygon.append(min(proy_origin_first_polygon))
            proyections_first_polygon.append(max(proy_origin_first_polygon))

            for side_second in second_sides:
                point = side_second.point_a
                vec = Vector(origin_point, point)
                proy = (prod_escalar(vec.vec, perpen_plane.vec) /
                        mod_perpen_plane)

                proy_origin_second_polygon.append(proy)

            proyections_second_polygon.append(min(proy_origin_second_polygon))
            proyections_second_polygon.append(max(proy_origin_second_polygon))

            min_first = proyections_first_polygon[0]
            max_first = proyections_first_polygon[1]
            min_second = proyections_second_polygon[0]
            max_second = proyections_second_polygon[1]

            if ((max_first >= min_second) and (max_second < min_first)) or (max_first < min_second):
                res = False
    return res


    # ******************** INIT PROGRAM ******************
polygon_list = []
for i in range(2):
    point_list = []

    for x in range(10):
        point = Point(random.randint(0, 30),
                      random.randint(0, 30))
        point_list.append(point)

    polygon = Polygon(point_list)
    convex_hull = polygon.getConvexHull()

    x_coor = []
    y_coor = []

    for point in polygon.point_list:
        x_coor.append(point.x)
        y_coor.append(point.y)

    plt.plot(x_coor, y_coor, "bo")

    convex_hull.drawPolygon()
    polygon_list.append(convex_hull)

is_collision = calcCollision(polygon_list)

plt.title("{}: {}".format("COLLISION", is_collision), fontsize=12)
plt.show()
