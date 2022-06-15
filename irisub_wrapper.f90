module irisubmodule
  use iso_c_binding
  implicit None

  real(c_float), bind(C) :: outf (20,1000) 
  real(c_float), bind(C) :: oar  (100)

  contains
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    subroutine read_namelist(jmag, xlat, xlon, iy, imd, hour, iut, &
         &     vbeg, vend, vstp ) bind(c)
      integer (c_int)   :: jmag, iy, imd, iut
      real    (c_float) :: xlat, xlon, hour, vbeg, vend, vstp
      integer           :: i
      namelist / NMIRI / jmag, xlat, xlon, iy, imd, hour, iut, vbeg, vend, vstp
      !
      !
      open(1,file='NMPARA')
      read(1, NMIRI)
      close(1)

      return
    end subroutine read_namelist

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    subroutine iri_sub_wrapper(jf_c, jmag, xlat, xlon, iy, imd, hour,&
         & vbeg, vend, vstp ) bind(C)
      logical (c_bool)  :: jf_c (50)  ! 1-byte
      logical           :: jf(50)     ! 4-byte
      integer (c_int)   :: jmag, iy, imd
      real    (c_float) :: xlat, xlon, hour, vbeg, vend, vstp
      integer :: jmag_f, iy_f, imd_f
      real    :: xlat_f, xlon_f, hour_f, vbeg_f, vend_f, vstp_f

      jf(:) = jf_c(:)
      call read_ig_rz
      call readapf107
      call iri_sub(jf,jmag,xlat,xlon,iy,imd,hour,vbeg,vend,vstp,outf,oar)

      return
    end subroutine iri_sub_wrapper
end module irisubmodule
