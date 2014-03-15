#
# plotStuff plotBump3d
#
# $show="twoBump3d2.show";
# $show="twoBump3d4.show";
# $show="twoBump3d8.show";
$show="bump3d8.show";
# 
$show
#
previous
contour
  delete contour plane 1
  contour lines 0
  plot the grid
    coarsening factor 8
    plot block boundaries 0
    toggle grid 0 0
    toggle grid 1 0
#
    toggle boundary 0 0 2 0
    toggle boundary 1 0 2 0
    toggle boundary 0 1 2 0
    toggle boundary 1 1 2 0
    toggle boundary 0 2 2 0
    toggle boundary 1 2 2 0
#
    toggle grid lines on boundary 0 0 2 0
    toggle grid lines on boundary 1 0 2 0
    toggle grid lines on boundary 0 1 2 0
    toggle grid lines on boundary 1 1 2 0
#
    toggle boundary 0 0 3 0
    toggle boundary 1 0 3 0
    toggle boundary 0 1 3 0
    toggle boundary 1 1 3 0
    toggle boundary 0 2 3 0
    toggle boundary 1 2 3 1
#
    toggle grid lines on boundary 0 0 3 0
    toggle grid lines on boundary 1 0 3 0
    toggle grid lines on boundary 0 1 3 0
    toggle grid lines on boundary 1 1 3 0
#
    exit this menu
    y+r 30
    x+r 20

  exit
previous
previous
reset:0
next
next
next
next
next
next
next
next
y+r:0
y+r:0
y+r:0
y+r:0
erase
contour
