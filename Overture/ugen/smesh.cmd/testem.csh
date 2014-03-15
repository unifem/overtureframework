#!/usr/bin/csh

foreach f ( trapzoid trapzoid2 c star bubbles boxinbox millermesh millermesh2 millermesh3 t millertfi brandon s.cmd )
  echo $f
  smesh $f.cmd
  $Overture/bin/ogen ot.cmd
  cp test.msh $f.msh
end

# smesh trapzoid2.cmd
# smesh c.cmd
# smesh star.cmd
# smesh bubbles.cmd
# smesh boxinbox.cmd
# smesh millermesh.cmd
# smesh millermesh2.cmd
# smesh millermesh3.cmd
# smesh t.cmd
