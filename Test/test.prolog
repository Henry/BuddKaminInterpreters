(define parent (X Y)
  (or
   (and (:=: X alice) (:=: Y sally))
   (and (:=: X sam) (:=: Y alice))
   ))

(query (and (parent X alice) (print X)))

(query (and (parent alice X) (print X)))

(query (and (parent fred X) (print X)))

(define grandparent (X Y)
  (and (parent X Z) (parent Z Y)))

(query (and (grandparent A B) (print A) (print B)))

(define fail () (:=: apples oranges))

(query (and (parent X Y) (print X) (fail)))

(define same (X Y) (:=: X Y))

(query (and (same A B) (:=: A sally) (print B)))

(define grandparent (X Y)
  (and (parent X Z) (parent Z Y)))

(query (and (grandparent A B) (print A) (print B)))

(query (and (grandparent sam A) (print A)))

quit
