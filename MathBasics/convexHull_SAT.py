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

    # Si no encuentra un punto que cumpla la condición de ComvexHull, se hacen las llamadas recursivas
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


# def calcCollision(polygon_list):

#     max_right_point_pol_one = getMaxRightPoint(polygon_list[0].point_list)
#     max_right_point_pol_two = getMaxRightPoint(polygon_list[1].point_list)

#     pol_left = 0
#     pol_right = 1

#     if (max_right_point_pol_one.x > max_right_point_pol_two.x):
#         pol_left = 1
#         pol_right = 0

#     print("*********************************************")
#     print("POL LEFT: {}".format(pol_left))
#     print("POL RIGHT: {}".format(pol_right))

#     max_right_point = getMaxRightPoint(polygon_list[pol_left].point_list)
#     max_left_point = getMaxLeftPoint(polygon_list[pol_right].point_list)
#     max_top_point = getMaxTopPoint(polygon_list[pol_left].point_list)
#     max_bottom_point = getMaxBottomPoint(polygon_list[pol_right].point_list)

#     print("*********************************************")
#     print("MAX RIGHT POINT: {}".format(max_right_point))
#     print("MAX LEFT POINT: {}".format(max_left_point))
#     print("MAX TOP POINT: {}".format(max_top_point))
#     print("MAX BOTTOM POINT: {}".format(max_bottom_point))
#     print("*********************************************")

#     vector_module_x = max_left_point.x - max_right_point.x

#     print("Distance X AXIS: {}".format(vector_module_x))
#     if vector_module_x <= 0:
#         vector_module_y = max_bottom_point.y - max_top_point.y
#         plt.xlabel("{}: {}".format("Collision", True))
#         print("Distance Y AXIS: {}".format(vector_module_y))

#         if vector_module_y <= 0:
#             plt.ylabel("{}: {}".format("Collision", True))

#         plt.ylabel("{}: {}".format("Collision", "Not important!"))
#         return True

#     plt.xlabel("{}: {}".format("Collision", False))
#     plt.ylabel("{}: {}".format("Collision", False))
#     return False


# def getMaxLeftPoint(list):
#     min = 100000
#     max = -100000
#     ptoRes = Point(min, max)
#     for pto in list:
#         if pto.x < min:
#             ptoRes = pto
#             min = pto.x
#     return ptoRes


# def getMaxRightPoint(list):
#     min = 100000
#     max = -100000
#     ptoRes = Point(min, max)
#     for pto in list:
#         if pto.x > max:
#             ptoRes = pto
#             max = pto.x
#     return ptoRes


# def getMaxBottomPoint(list):
#     min = -100000
#     max = 100000
#     ptoRes = Point(min, max)
#     for pto in list:
#         if pto.y < max:
#             ptoRes = pto
#             max = pto.y
#     return ptoRes


# def getMaxTopPoint(list):
#     min = -100000
#     max = -100000
#     ptoRes = Point(min, max)
#     for pto in list:
#         if pto.y > max:
#             ptoRes = pto
#             max = pto.y
#     return ptoRes

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
    collisions = []
    res = False
    polygon = polygon_list[0]
    first_sides = polygon.get_sides()
    polygon = polygon_list[1]
    second_sides = polygon.get_sides()
    init_side = 0
    plane_proyection = Vector(Point(0, 0), Point(0, 0))

    for side in first_sides:
        init_side = side

        plane_proyection.vec = init_side.change_vector()  # REVISAR

        all_projections_polygon_one = []
        min_proy_first_polygon = 10000
        max_proy_first_polygon = -10000
        projection_one_all = [0.0, 0.0]

        print(init_side.point_a)
        print(init_side.point_b)
        print(plane_proyection.vec)
        print("-----------------")

        for test_side in first_sides:

            numer = prod_escalar(test_side.vec, plane_proyection.vec)

            mod_plane = plane_proyection.vector_module()
            first_part = 0
            if (mod_plane != 0):
                first_part = numer / mod_plane

            pto_init = plane_proyection.point_a
            pto_fin = Point((plane_proyection.vec[0] * first_part) / mod_plane,
                            (plane_proyection.vec[1] * first_part) / mod_plane)

            if pto_init.x > pto_fin.x:
                result = [pto_fin, pto_init]
            else:
                result = [pto_init, pto_fin]

            if result[0].x < min_proy_first_polygon:
                min_proy_first_polygon = result[0].x

            if result[1].x > max_proy_first_polygon:
                max_proy_first_polygon = result[1].x

            print("+++++++++++++++++")
            print(result)
            print("-----------------")
            # if len(all_projections_polygon_one) > 0:
            #     if result[0] < min_proy_first_polygon:
            #         min_proy_first_polygon = result[0]
            #     if result[1] > max_proy_first_polygon:
            #         max_proy_first_polygon = result[1]

            all_projections_polygon_one.append(result)
            # print("SIDE: {}".format(test_side))
            # print("PLANE: {}".format(plane_proyection))
            # print("X COOR PLANE: {}".format(plane_proyection.vec[0]))
            # print("Y COOR PLANE: {}".format(plane_proyection.vec[1]))
            # print("NUMERATOR: {}".format(numer))
            # print("DENOMINATOR: {}".format(mod_plane))
            # print("************************************")

        print("+++++++++++++++++")
        print([min_proy_first_polygon, max_proy_first_polygon])
        print("-----------------")

        all_coordenates_polygon_two = []
        min_proy_second_polygon = 10000
        max_proy_second_polygon = -10000
        first_point_second_polygon = second_sides[0].point_a

        for test_side in second_sides:
            numer = prod_escalar(test_side.vec, plane_proyection.vec)

            mod_plane = plane_proyection.vector_module()
            first_part = 0
            if (mod_plane != 0):
                first_part = numer / mod_plane

            pto_init = first_point_second_polygon

            pto_fin = Point(((plane_proyection.vec[0] * first_part) / mod_plane) + first_point_second_polygon.x,
                            (plane_proyection.vec[1] * first_part) / mod_plane)

            if pto_init.x > pto_fin.x:
                result = [pto_fin, pto_init]
            else:
                result = [pto_init, pto_fin]

            if result[0].x < min_proy_second_polygon:
                min_proy_second_polygon = result[0].x

            if result[1].x > max_proy_second_polygon:
                max_proy_second_polygon = result[1].x

            all_coordenates_polygon_two.append(result)

            # print("SIDE: {}".format(test_side))
            # print("PLANE: {}".format(plane_proyection))
            # print("X COOR PLANE: {}".format(plane_proyection.vec[0]))
            # print("Y COOR PLANE: {}".format(plane_proyection.vec[1]))
            # print("NUMERATOR: {}".format(numer))
            # print("DENOMINATOR: {}".format(mod_plane))
            # print("************************************")
        print("************************************")
        print("POLYGON ONE RESULT: {}".format(
            [min_proy_first_polygon, max_proy_first_polygon]))
        print("POLYGON TWO RESULT: {}".format(
            [min_proy_second_polygon, max_proy_second_polygon]))
        # print("RESULT ONE: {}".format(first_polygon))
        # print("RESULT TWO: {}".format(second_polygon))
        # print("************************************")
        res = True
        collisions.append([max_proy_first_polygon,
                           min_proy_second_polygon])
        if max_proy_first_polygon < min_proy_second_polygon:
            res = False
            median = ((min_proy_second_polygon - max_proy_first_polygon) /
                      2) + min_proy_second_polygon
            median_point = Point(median, plane_proyection.vec[1])
            if plane_proyection.vec[1] != 0:
                slope = -(plane_proyection.vec[0] / plane_proyection.vec[1])
            else:
                slope = 0

            order = median_point.y - (slope * median)

            if slope == 0:
                pto_sup = Point(0, 20)
                pto_inf = Point(0, -20)
            else:
                pto_sup = Point((20-order)/slope, 20)
                pto_inf = Point((-20-order)/slope, -20)

            plt.plot(median_point.x, median_point.y, "yo")
            plt.plot([pto_inf.x, pto_sup.x], [pto_inf.y, pto_sup.y],
                     "go-", linewidth=1, markersize=6)
            break
        if res == False:
            return res
    print("RESULT:***************")
    print(collisions)
    return res


def check_collision(polygon_one, polygon_two):
    collision = False

    return collision

    # ******************** INIT PROGRAM ******************
polygon_list = []
for i in range(2):
    point_list = []

    for x in range(10):
        point = Point(random.randint((10 * i) - 3 * i, 10 * (i + 1)),
                      random.randint(10 * i, 10 * (i + 1)))
        # point = Point(random.randint(0, 30),
        #               random.randint(0, 30))
        point_list.append(point)

    polygon = Polygon(point_list)
    convex_hull = polygon.getConvexHull()

    print("{}: {}".format("POLYGON", i))
    print(polygon)
    print("{}: {}".format("CONVEX HULL", i))
    print(convex_hull)

    x_coor = []
    y_coor = []

    for point in polygon.point_list:
        x_coor.append(point.x)
        y_coor.append(point.y)

    plt.plot(x_coor, y_coor, "bo")

    convex_hull.drawPolygon()
    polygon_list.append(convex_hull)

print("*********************************************")
print("CALC COLLISION")
is_collision = calcCollision(polygon_list)
print("*********************************************")

print("{}: {}".format("COLLISION", is_collision))
plt.title("{}: {}".format("COLLISION", is_collision), fontsize=12)
plt.show()
